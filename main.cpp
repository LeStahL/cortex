#include "Window.hpp"

int WINAPI demo(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Window win(hInstance, "Test demo.");
    win.showSelector();
	
	return 0;
}
