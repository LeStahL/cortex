#include "SymbolTable.hpp"

#include <cstring>

SymbolTable::SymbolTable()
    : shaders(nullptr)
    , nShaders(0)
{
    symbolTable = this;
}

bool SymbolTable::hasSymbol(const char* symbol)
{
    for(int i=0; i<nShaders; ++i)
        if(!strcmp(symbol, shaders[i]->symbol)) return true;
    return false;
}

void SymbolTable::addSymbol(Shader* shader)
{
    if(!hasSymbol(shader->symbol))
    {
        ++nShaders;
        if(nShaders == 1) shaders = (Shader**)malloc(sizeof(Shader*));
        else shaders = (Shader**)realloc(shaders, nShaders*sizeof(Shader*));
        shaders[nShaders-1] = shader;
    }
}

void SymbolTable::compileContainedSymbols()
{
    for(int i=0; i<nShaders; ++i)
        if(!shaders[i]->isCompiled) shaders[i]->compile();
}

int SymbolTable::nCompiledShaders()
{
    int nCompiledShaders = 0;
    for(int i=0; i<nShaders; ++i)
        if(shaders[i]->isCompiled) ++nCompiledShaders;
    return nCompiledShaders;
}
