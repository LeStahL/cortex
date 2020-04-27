#pragma once

#include "Build.gen.hpp"
#include "Shader.hpp"

static class SymbolTable
{
public:
    SymbolTable();
    
    bool hasSymbol(const char *symbol);
    void addSymbol(Shader *shader);
    void compileContainedSymbols();
    int nCompiledShaders();
    
    Shader **shaders;
    int nShaders;
};
