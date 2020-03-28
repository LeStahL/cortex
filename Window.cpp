#include "Window.hpp"

#include <cstring>
#include <cstdlib>
#include <cstdio>

LRESULT CALLBACK callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UINT id = LOWORD(wParam);
    
    switch(uMsg)
	{
        case WM_COMMAND:
            switch(id)
            {
                case START_BUTTON:
                    // TODO: Start demo here
                    break;
            }
            break;
            
        case WM_CLOSE:
			ExitProcess(0);
			break;
            
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
            break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::Window(HINSTANCE _instance, const char* _title)
    : title(_title)
    , instance(_instance)
    , isSelector(true)
{
    // Determine supported display device modes
    DEVMODE dm = { 0 };
    dm.dmSize = sizeof(dm);
    
    // Get number of supported device modes
    for(nResolutions = 0; EnumDisplaySettings(NULL, nResolutions, &dm) != 0; ++nResolutions);
    
    // Allocate arrays
    int *widths = (int*) malloc(nResolutions * sizeof(int)),
        *heights = (int*) malloc(nResolutions * sizeof(int)),
        *rates = (int*)malloc(nResolutions * sizeof(int)),
        defaultIndex = 0;
    
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
	wc.lpfnWndProc = &callback;
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
    
    handle = CreateWindowEx(0, title, title, WS_OVERLAPPEDWINDOW, 200, 200, 300, 360, NULL, NULL, instance, 0);
    
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

void Window::showSelector()
{
    MSG msg = { 0 };
	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

