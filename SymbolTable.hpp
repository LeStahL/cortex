#pragma once

#include "Build.gen.hpp"
#include "Shader.hpp"

static class SymbolTable
{
public:
    SymbolTable();
    
    bool hasSymbol(const char *symbol) const;
    void addSymbol(Shader *shader);
    void compileContainedSymbols();
    int nCompiledShaders();
    Shader *shaderBySymbolName(const char *symbol) const;

    Shader **shaders;
    int nShaders;
};
