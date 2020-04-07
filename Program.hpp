#pragma once

#include <Windows.h>
#include "gl/GL.h"

#include "Build.gen.h"

class Program
{
public:
    Program(Shader *shaders = 0);
    
    void attachShader(Shader *shader);
    void link();
    
    GLuint handle;
    Shader *shaders;
    int nShaders;
#ifdef DEBUG
    bool hasLinkError();
    char *linkError();
#endif // DEBUG
};
