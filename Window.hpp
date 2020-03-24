#pragma once

#include "Windows.h"

class Window
{
public:
    Window(const char *title = " :: Team210 - Go, make a demo :: ");
    ~Window();
    
    const char *title;
    HWND handle, 
        *controlHandles;
    HDC deviceContext;
    HINSTANCE instance;
};

static Window *windowInstance = nullptr;
