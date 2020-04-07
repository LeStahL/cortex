#pragma once

#include "Build.gen.hpp"
#include "Shader.hpp"

static class SymbolTable
{
public:
    SymbolTable();
    
    void addSymbol(Shader *shader);
    void compileContainedSymbols();
    
    Shader *symbols;
    int nSymbols;
} *symbolTable = nullptr;
