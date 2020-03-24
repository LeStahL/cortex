#include "Window.hpp"

int WINAPI demo(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Window win("Test demo.");
    
    MSG msg = { 0 };
	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 0;
}
