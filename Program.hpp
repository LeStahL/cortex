#pragma once

#include <Windows.h>
#include "gl/GL.h"

#include "Build.gen.hpp"
#include "Shader.hpp"

class Program
{
public:
    Program();
    
    void attachShader(Shader *shader);
    void link();
    
    GLuint handle;
    Shader **shaders;
    int nShaders;
#ifdef DEBUG
    bool hasLinkError();
    char *linkError();
#endif // DEBUG
};
