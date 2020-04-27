#pragma once

#include "Build.gen.hpp"
#include "Configuration.hpp"
#include "Demo.hpp"
#include "LoadingBar.hpp"

#ifdef MSVC
#include "Windows.h"
#include <commctrl.h>

#define RESOLUTION_COMBOBOX 0x1
#define MUTE_CHECKBOX 0x2
#define SFX_BUFFERSIZE_COMBOBOX 0x3
#define RECORD_CHECKBOX 0x4
#define SFX_SAMPLERATE_COMBOBOX 0x5
#define RECORD_OUTPUT_FILENAME_EDIT 0x6
#define RECORD_FRAMERATE_COMBOBOX 0x7
#define START_BUTTON 0x8

class Window
{
public:
    Window(HINSTANCE instance, const char *title = " :: Team210 - Go, make a demo :: ");
    ~Window();

    LRESULT CALLBACK callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    // Window related
    const char *title;
    HWND handle;
    HDC deviceContext;
    HINSTANCE instance;
    
    // OpenGL related
    HGLRC glrc;
    
    // Selector related
    bool isSelector,
        recording;
    const char *outputDirectory;
    int nResolutions = 0,
        nUniqueResolutions = 0,
        *widths,
        *heights,
        *rates;
    HWND resolutionLabelHandle,
        resolutionDropdownHandle,
        muteCheckboxHandle,
        sfxBufferSizeLabelHandle,
        sfxBufferSizeDropdownHandle,
        sfxSampleRateLabelHandle,
        sfxSampleRateDropdownHandle,
        recordCheckboxHandle,
        recordOutputDirectoryLabelHandle,
        recordOutputDirectoryTextboxHandle,
        recordOutputFramerateLabelHandle,
        recordOutputFramerateDropdownHandle,
        startButtonHandle,
        team210CreditLabel;

    // Demo related
    bool isRunning;
    Configuration configuration;
    LoadingBar *loadingBar;
    Demo *demo;
    
    void initializeOpengl();
    void showSelector();
    void showLoadingBar(LoadingBar *loadingBar);
    void showDemo(Demo *demo);
    int flipBuffers();
    void quad();
};
#endif // MSVC
