#pragma once

#include "gl/GL.h"

class Shader
{
public:
    Shader(const char *source);
    virtual ~Shader();
    
    GLuint handle;
    bool hasCompileError();
    char *compileError();
};
