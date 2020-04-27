#pragma once

#include "Shader.hpp"
#include "Program.hpp"
// #include "Buffer.hpp"
#include "SymbolTable.hpp"

class LoadingBar
{
public:
    LoadingBar(SymbolTable *symbolTable);
    
    Shader *ownShader;
    Program *ownProgram;
    SymbolTable *symbolTable;
    
    // void addCompileShaderTask(Shader *shader);
    // void addLinkProgramTask(Program *program);
    // void addPrecalcBufferTask(Buffer *buffer);
    
    // float progress,
        // *taskStatus;
    // typedef enum TaskType
    // {
    //     CompileShader,
    //     LinkProgram,
    //     PrecalcBuffer
    // };
    // TaskType *taskType;
    
    // void update();
};
