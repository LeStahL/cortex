#pragma once

class Configuration
{
public:
    Configuration();
    
    int screenWidth,
        screenHeight,
        sfxBufferWidth,
        sfxSampleRate,
        recordFps;
    bool muted,
        record;
    char recordDirname[1024];
};
