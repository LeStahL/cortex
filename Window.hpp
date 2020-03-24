#pragma once

#include "Windows.h"
#	include <commctrl.h>
#define RESOLUTION_COMBOBOX 0x1

class Window
{
public:
    Window(const char *title = " :: Team210 - Go, make a demo :: ");
    ~Window();
    
    // Window related
    const char *title;
    HWND handle, 
        *controlHandles;
    HDC deviceContext;
    HINSTANCE instance;
    
    // OpenGL related
    
    // Selector related
    bool isSelector,
        recording;
    const char *outputDirectory;
    int nResolutions = 0,
        nUniqueResolutions = 0;
    HWND resolutionLabelHandle,
        resolutionDropdownHandle,
        muteCheckboxHandle,
        sfxBufferSizeLabelHandle,
        sfxBufferSizedropdownHandle,
        recordCheckboxHandle,
        recordOutputDirectoryLabelHandle,
        recordOutputDirectoryTextboxHandle,
        recordOutputFramerateLabelHandle,
        recordOutputFramerateDropdownHandle,
        startButtonHandle,
        team210CreditLabel;
};

static Window *windowInstance = nullptr;
