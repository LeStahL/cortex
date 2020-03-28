#include "Build.gen.hpp"
#include "Window.hpp"

#include <cstdio>

#ifdef MSVC
int WINAPI demo(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
#ifdef DEBUG
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif // DEBUG
    
    Window win(hInstance, "Test demo.");
    win.showSelector();
	
	return 0;
}
#endif // MSVC
