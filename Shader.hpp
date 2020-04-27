#pragma once

#include <Windows.h>
#include "gl/GL.h"

#include "Build.gen.hpp"

class Shader
{
public:
    Shader(const char *source, const char *symbol = nullptr);
    
    void compile();
    bool isCompiled;
    bool isSymbol;
    
    GLuint handle;
    char *symbol;

#ifdef DEBUG
    bool hasCompileError();
    char *compileError();
#endif // DEBUG
};
