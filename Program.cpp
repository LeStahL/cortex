#include "Program.hpp"
#include "OpenGL.hpp"

Program::Program(Shader *_shaders)
    : shaders(_shaders)
{
    handle = glCreateProgram();
}

#ifdef DEBUG
bool Program::hasLinkError()
{
    GLint linkStatus;
    glGetShaderiv(handle, GL_LINK_STATUS, &linkStatus);
    return linkStatus == GL_TRUE;
}

char *Program::linkError()
{
    int infoLogLenght;
    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &infoLogLenght);
    char *infoLog = (char *) malloc(infoLogLenght);
    glGetProgramInfoLog(handle, infoLogLenght, NULL, infoLog);
    return infoLog;
}
#endif // DEBUG

void Program::attachShader(Shader* shader)
{
    ++nShaders;
    if(shaders == 0) shaders = (Shader*)malloc(sizeof(Shader));
    else shaders = (Shader*)realloc(shaders, nShaders*sizeof(Shader));
    shaders[nShaders-1] = shader;
    glAttachShader(handle, shader.handle);
}

void Program::link()
{
    glLinkProgram(handle);
}
