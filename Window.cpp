#include "Window.hpp"

LRESULT CALLBACK callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
	{
        case WM_CLOSE:
			ExitProcess(0);
			break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::Window(const char* _title)
    : title(_title)
{
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
	wc.lpszClassName = this->title;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);
    
    this->handle = CreateWindowEx(0, this->title, this->title, WS_OVERLAPPEDWINDOW, 200, 200, 341, 150, NULL, NULL, this->instance, 0);
    
    ShowWindow(this->handle, TRUE);
    UpdateWindow(this->handle);
    
    windowInstance = this;
}

Window::~Window()
{
}
