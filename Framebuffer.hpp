#pragma once

class Framebuffer
{
    public:
    Framebuffer(const char *name);
    void bind();

    int handle;
    char *name;
};