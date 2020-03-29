#pragma once

class Configuration
{
public:
    Configuration();
    
    int screenWidth,
        screenHeight,
        screenRate,
        sfxBufferWidth,
        sfxSampleRate,
        recordFps;
    bool muted,
        record;
    char recordDirname[1024];
};
