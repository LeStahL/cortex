#pragma once

#include "Shader.hpp"
#include "Program.hpp"
#include "Buffer.hpp"

class LoadingBar
{
public:
    LoadingBar();
    
    void addCompileShaderTask(Shader *shader);
    void addLinkProgramTask(Program *program);
    void addPrecalcBufferTask(Buffer *buffer);
    
    float progress,
        *taskStatus;
    Shader *shader;
    typedef enum TaskType
    {
        CompileShader,
        LinkProgram,
        PrecalcBuffer
    };
    TaskType *taskType;
    
    void update();
};
