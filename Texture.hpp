#pragma once

#include "OpenGL.hpp"

static int TEXTURE_ID = 0;

class Texture
{
    public:
    Texture(const char *name, int width, int height);
    void bind();

    char *name;
    void *data;
    int handle,
        width,
        height;
};