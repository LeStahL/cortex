#include "LoadingBar.hpp"

void LoadingBar::addCompileShaderTask(Shader *shader)
{
    if(shader->isSymbol) symbolTable->addSymbol(shader);
    else
    {
        ++nShaderTasks;
        if(nShaderTasks == 1) shaderTasks = (Shader **) malloc(sizeof(Shader *));
        else shaderTasks = (Shader **) realloc(shaderTasks, nShaderTasks*sizeof(Shader *));
        shaderTasks[nShaderTasks-1] = shader;
    }
    
}

void LoadingBar::addLinkProgramTask(Program *program)
{
    ++nProgramTasks;
    if(nProgramTasks == 1) programTasks = (Program **) malloc(sizeof(Program *));
    else programTasks = (Program **) realloc(programTasks, nProgramTasks * sizeof(Program *));
    programTasks[nProgramTasks - 1] = program;
}

void LoadingBar::performAllTasks()
{
    // Compile symbols first
    symbolTable->compileContainedSymbols();

    // Compile non-symbolic shaders
    for(int i=0; i<nShaderTasks; ++i)
        shaderTasks[i]->compile();
    
    // Link programs
    for(int i=0; i<nProgramTasks; ++i)
        programTasks[i]->link();
}