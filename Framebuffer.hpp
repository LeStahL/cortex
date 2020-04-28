#pragma once

class Framebuffer
{
    public:
    Framebuffer(const char *name);

    int handle;
    char *name;
};