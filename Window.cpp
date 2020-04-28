#include "Window.hpp"
#include "OpenGL.hpp"

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#ifdef MSVC
LRESULT CALLBACK static_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UINT id = LOWORD(wParam);
    
    if(uMsg == WM_NCCREATE)
    {
        LPVOID thisPointer = ((CREATESTRUCT *)lParam)->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG) thisPointer);
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    else
    {
        Window *demoWindow = (Window *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
        return demoWindow->callback(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT CALLBACK Window::callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UINT id = LOWORD(wParam);
    switch(uMsg)
	{
        case WM_COMMAND:
            switch(id)
            {
                case START_BUTTON:
                {
                    this->isSelector = false;
                    break;
                }   
                case MUTE_CHECKBOX:
                {
                    int checked = SendMessage(this->muteCheckboxHandle, BM_GETCHECK, 0, 0);
                    SendMessage(this->muteCheckboxHandle, BM_SETCHECK, checked?BST_UNCHECKED:BST_CHECKED, 0);
                    EnableWindow(this->sfxBufferSizeLabelHandle, checked);
                    EnableWindow(this->sfxBufferSizeDropdownHandle, checked);
                    EnableWindow(this->sfxSampleRateLabelHandle, checked);
                    EnableWindow(this->sfxSampleRateDropdownHandle, checked);
                    break;
                }
                case RECORD_CHECKBOX:
                {
                    int checked = SendMessage(this->recordCheckboxHandle, BM_GETCHECK, 0, 0);
                    SendMessage(this->recordCheckboxHandle, BM_SETCHECK, checked?BST_UNCHECKED:BST_CHECKED, 0);
                    EnableWindow(this->recordOutputDirectoryLabelHandle, !checked);
                    EnableWindow(this->recordOutputDirectoryTextboxHandle, !checked);
                    EnableWindow(this->recordOutputFramerateLabelHandle, !checked);
                    EnableWindow(this->recordOutputFramerateDropdownHandle, !checked);
                    break;
                }
            }
            break;
        case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					ExitProcess(0);
					break;

				case VK_SPACE:
					// pause/unpaused render timer
					// paused = !paused;
					// if(paused)
					// 	waveOutPause(hWaveOut);
					// else
					// 	waveOutRestart(hWaveOut);
					break;
// #ifdef DEBUG
//                 case VK_RETURN:
//                     showDebugWindow = !showDebugWindow;
//                     break;
// #endif
			}
			break;
		
        case WM_RBUTTONDOWN: // Breakpoint / UNC ruleset
			ExitProcess(0);
			break;

        case WM_CLOSE:
			ExitProcess(0);
			break;
            
        case WM_PAINT:
            if(isSelector)
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
                EndPaint(hwnd, &ps);
            }
            break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::Window(HINSTANCE _instance, const char* _title)
    : title(_title)
    , instance(_instance)
    , isSelector(true)
    , isRunning(true)
{
    // Determine supported display device modes
    DEVMODE dm = { 0 };
    dm.dmSize = sizeof(dm);
    
    // Get number of supported device modes
    for(nResolutions = 0; EnumDisplaySettings(NULL, nResolutions, &dm) != 0; ++nResolutions);
    
    // Allocate arrays
    widths = (int*) malloc(nResolutions * sizeof(int));
    heights = (int*) malloc(nResolutions * sizeof(int));
    rates = (int*)malloc(nResolutions * sizeof(int));
    int defaultIndex = 0;
    
    // Fill arrays
    for(int i = 0; i < nResolutions; ++i) 
    {
        EnumDisplaySettings(NULL, i, &dm);
        
        // Check if settings are already in list
        bool isInList = false;
        for(int k = 0; k < i; ++k)
        {
            if(widths[k] == dm.dmPelsWidth && heights[k] == dm.dmPelsHeight && rates[k] == dm.dmDisplayFrequency)
            {
                isInList = true;
                break;
            }
        }
        
        // Add to list if not present
        if(!isInList)
        {
            widths[nUniqueResolutions] = dm.dmPelsWidth;
            heights[nUniqueResolutions] = dm.dmPelsHeight;
            rates[nUniqueResolutions] = dm.dmDisplayFrequency;
            
            // Mark default (720p)
            if(dm.dmPelsWidth == 1280 && dm.dmPelsHeight == 720 && dm.dmDisplayFrequency == 60)     
                defaultIndex = nUniqueResolutions;
            
            ++nUniqueResolutions;
        }
    }
    
    // Build selector window
    WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = &static_callback;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->instance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = title;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);
    
    handle = CreateWindowEx(0, title, title, WS_OVERLAPPEDWINDOW, 200, 200, 300, 360, NULL, NULL, instance, (LPVOID) this);

    // Add components to selector window
    resolutionLabelHandle = CreateWindow(WC_STATIC, "Resolution: ", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 12, 80, 15, handle, NULL, instance, NULL);
    
    resolutionDropdownHandle = CreateWindow(WC_COMBOBOX, "", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 100, 10, 175, nUniqueResolutions*15, handle, (HMENU)RESOLUTION_COMBOBOX, instance, NULL);
    
    // Add items to resolution combo box and select full HD
    for(int i=0; i<nUniqueResolutions; ++i)
    {
        char name[1024];
        sprintf(name, "%d x %d @ %d Hz", widths[i], heights[i], rates[i]);
        SendMessage(resolutionDropdownHandle, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) name);
    }
	SendMessage(resolutionDropdownHandle, CB_SETCURSEL, defaultIndex, 0);
    
    // Add mute checkbox
    muteCheckboxHandle = CreateWindow(WC_BUTTON, "Mute demo", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10,40,265,15, handle, (HMENU) MUTE_CHECKBOX, instance, NULL);
    
    // Add SFX buffer size selector
    sfxBufferSizeLabelHandle = CreateWindow(WC_STATIC, "SFX buffer: ", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 64, 80, 15, handle, NULL, instance, NULL);
    
    sfxBufferSizeDropdownHandle = CreateWindow(WC_COMBOBOX, "", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 100, 62, 175, nUniqueResolutions*15, handle, (HMENU)SFX_BUFFERSIZE_COMBOBOX, instance, NULL);
    
    int size = 128;
    for(int i=0; i<4; ++i)
    {
        char name[1024];
        sprintf(name, "%d*%d px", size, size);
        SendMessage(sfxBufferSizeDropdownHandle, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) name);
        size *= 2;
    }
	SendMessage(sfxBufferSizeDropdownHandle, CB_SETCURSEL, 2, 0);
    
    // Add sample rate selector
    sfxSampleRateLabelHandle = CreateWindow(WC_STATIC, "SFX rate: ", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 94, 80, 15, handle, NULL, instance, NULL);
    
    sfxSampleRateDropdownHandle = CreateWindow(WC_COMBOBOX, "", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 100, 92, 175, 60, handle, (HMENU)SFX_SAMPLERATE_COMBOBOX, instance, NULL);
    
    SendMessage(sfxSampleRateDropdownHandle, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) "44.1 kHz");
    SendMessage(sfxSampleRateDropdownHandle, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) "48 kHz");
	SendMessage(sfxSampleRateDropdownHandle, CB_SETCURSEL, 0, 0);
    
    // Add record checkbox
    recordCheckboxHandle = CreateWindow(WC_BUTTON, "Record demo", WS_VISIBLE | WS_CHILD | BS_CHECKBOX, 10,122,265,15, handle, (HMENU) RECORD_CHECKBOX, instance, NULL);
    
    // Add record filename
    recordOutputDirectoryLabelHandle = CreateWindow(WC_STATIC, "Output: ", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 146, 80, 15, handle, NULL, instance, NULL);
    EnableWindow(recordOutputDirectoryLabelHandle, FALSE);
    
    recordOutputDirectoryTextboxHandle = CreateWindow(WC_EDIT, "recording", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 144, 175, 22, handle, (HMENU) RECORD_OUTPUT_FILENAME_EDIT, NULL, NULL);
    EnableWindow(recordOutputDirectoryTextboxHandle, FALSE);
    
    // Add record framerate selector
    recordOutputFramerateLabelHandle = CreateWindow(WC_STATIC, "Framerate: ", WS_VISIBLE | WS_CHILD | SS_LEFT, 10, 174, 80, 15, handle, NULL, instance, NULL);
    EnableWindow(recordOutputFramerateLabelHandle, FALSE);
    
    recordOutputFramerateDropdownHandle = CreateWindow(WC_COMBOBOX, "", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 100, 172, 175, 60, handle, (HMENU)RECORD_FRAMERATE_COMBOBOX, instance, NULL);
    EnableWindow(recordOutputFramerateDropdownHandle, FALSE);

    SendMessage(recordOutputFramerateDropdownHandle, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) "60 Fps");
    SendMessage(recordOutputFramerateDropdownHandle, (UINT) CB_ADDSTRING, (WPARAM) 0, (LPARAM) "30 Fps");
	SendMessage(recordOutputFramerateDropdownHandle, CB_SETCURSEL, 0, 0);
    
    // Team210 box
    team210CreditLabel = CreateWindow(WC_STATIC, "|-Team210 - we are ::\n|-QM :: Code^SFX\n|-NR4 :: Code^GFX\n|-Atlas :: GFX\n|-MIC :: Ideas^GFX\n|-Grenzdevil :: Dir\n|-DaDummy :: Code", WS_VISIBLE | WS_CHILD | SS_LEFT | WS_BORDER, 10, 202, 150, 113, handle, NULL, instance, NULL);
    
    // Start button
    startButtonHandle = CreateWindow(WC_BUTTON,"Offend!",WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,165,202,113,113,handle,(HMENU)START_BUTTON,instance,NULL);
    
    ShowWindow(handle, TRUE);
    UpdateWindow(handle);
}

Window::~Window()
{
}

int Window::flipBuffers()
{
    SwapBuffers(deviceContext);

	MSG msg = { 0 };
	while ( PeekMessageA( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		if ( msg.message == WM_QUIT ) {
			return FALSE;
		}
		TranslateMessage( &msg );
		DispatchMessageA( &msg );
	}

	return TRUE;
}

void Window::showSelector()
{
	while(isSelector) flipBuffers();
    
    int index;
	
    index = SendMessage(resolutionDropdownHandle, CB_GETCURSEL, 0, 0);
    configuration.screenWidth = widths[index];
    configuration.screenHeight = heights[index];
    configuration.screenRate = rates[index];
    
    index = SendMessage(sfxBufferSizeDropdownHandle, CB_GETCURSEL, 0, 0);
    configuration.sfxBufferWidth = 128*pow(2,index);
    
    index = SendMessage(sfxSampleRateDropdownHandle, CB_GETCURSEL, 0, 0);
    configuration.sfxSampleRate = index?48000:44100;
    
    index = SendMessage(recordOutputFramerateDropdownHandle, CB_GETCURSEL, 0, 0);
    configuration.recordFps = index?30:60;
    
    configuration.muted = SendMessage(muteCheckboxHandle, BM_GETCHECK, 0, 0);
    configuration.record = SendMessage(recordCheckboxHandle, BM_GETCHECK, 0, 0);
    
    GetWindowText(recordOutputDirectoryTextboxHandle, configuration.recordDirname, 1024);

    DestroyWindow(resolutionLabelHandle);
    DestroyWindow(resolutionDropdownHandle);
    DestroyWindow(muteCheckboxHandle);
    DestroyWindow(sfxBufferSizeLabelHandle);
    DestroyWindow(sfxBufferSizeDropdownHandle);
    DestroyWindow(sfxSampleRateLabelHandle);
    DestroyWindow(sfxSampleRateDropdownHandle);
    DestroyWindow(recordCheckboxHandle);
    DestroyWindow(recordOutputDirectoryLabelHandle);
    DestroyWindow(recordOutputDirectoryTextboxHandle);
    DestroyWindow(recordOutputFramerateLabelHandle);
    DestroyWindow(recordOutputFramerateDropdownHandle);
    DestroyWindow(startButtonHandle);
    DestroyWindow(team210CreditLabel);
}

void Window::initializeOpengl()
{
    LONG lStyle = GetWindowLong(handle, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    SetWindowLong(handle, GWL_STYLE, lStyle);

    SetWindowPos(handle, HWND_TOP, 0, 0, configuration.screenWidth, configuration.screenHeight, SWP_SHOWWINDOW);
    UpdateWindow(handle);

    DEVMODE dma = { 0 };
    dma.dmSize = sizeof(DEVMODE);
    dma.dmPelsWidth = configuration.screenWidth;
    dma.dmPelsHeight = configuration.screenHeight;
    dma.dmDisplayFrequency = configuration.screenRate;
    dma.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
    
    ChangeDisplaySettings(&dma, CDS_FULLSCREEN);
    
    ShowWindow(handle, TRUE);
    UpdateWindow(handle);
    
    PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

    deviceContext = GetDC(handle);

	int  pf = ChoosePixelFormat(deviceContext, &pfd);
	SetPixelFormat(deviceContext, pf, &pfd);

	glrc = wglCreateContext(deviceContext);
	wglMakeCurrent(deviceContext, glrc);

    ShowCursor(FALSE);
    
    initializeOpenGLExtensions();
}

void Window::showLoadingBar(LoadingBar *_loadingBar)
{
    loadingBar = _loadingBar;

    // TODO: track loading bar status here
    while(flipBuffers())
    {
    //     printf("draw()\n");
    //     // Draw loading bar here until finished loading
        glClearColor(1.,0.,0.,1.);
        glClear(GL_COLOR_BUFFER_BIT);

        loadingBar->ownProgram->use();
        loadingBar->ownProgram->uniform2f("iResolution", configuration.screenWidth, configuration.screenHeight);
        loadingBar->ownProgram->uniform1f("iProgress", .5);
        quad();
    }
}

void Window::showDemo(Demo *_demo)
{
    demo = _demo;

    while(flipBuffers())
    {
        // Draw demo here
    }
}

void Window::quad()
{
    glBegin(GL_QUADS);
    glVertex3f(-1,-1,0);
    glVertex3f(-1,1,0);
    glVertex3f(1,1,0);
    glVertex3f(1,-1,0);
    glEnd();
    glFlush();
}

#endif // MSVC
