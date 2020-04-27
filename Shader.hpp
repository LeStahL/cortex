#pragma once

#include <Windows.h>
#include "gl/GL.h"

#include "Build.gen.hpp"

class Shader
{
public:
    Shader(const char *source, const char *symbol);
    
    void compile();
    bool isCompiled;
    
    GLuint handle;
    char *symbol;
#ifdef DEBUG
    bool hasCompileError();
    char *compileError();
#endif // DEBUG
};
