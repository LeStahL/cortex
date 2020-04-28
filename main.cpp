#include "Build.gen.hpp"
#include "Window.hpp"
#include "LoadingBar.hpp"
#include "OpenGL.hpp"

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
	win.initializeOpengl();

	SymbolTable *symbolTable = new SymbolTable();
	LoadingBar *loadingBar = new LoadingBar(symbolTable, &win);
	// Demo *demo = new Demo(loadingBar);

	return 0;
}
#endif // MSVC
