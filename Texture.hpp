#pragma once

#include "OpenGL.hpp"

class Texture
{
    public:
    Texture(const char *name);

    char *name;
    int handle;
};