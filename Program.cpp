#include "Program.hpp"
#include "OpenGL.hpp"

#include <cstdio>

Program::Program(const char *_name)
    : nShaders(0)
    , shaders(nullptr)
    , uniformLocations(nullptr)
    , uniformNames(nullptr)
    , nUniforms(0)
    , isLinked(false)
{
    handle = glCreateProgram();
    name = (char *) malloc(strlen(_name) + 3);
    sprintf(name, "%s", _name);
}

#ifdef DEBUG
bool Program::hasLinkError()
{
    GLint linkStatus;
    glGetProgramiv(handle, GL_LINK_STATUS, &linkStatus);
    return linkStatus != GL_TRUE;
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
    if(shaders == nullptr) shaders = (Shader**)malloc(sizeof(Shader*));
    else shaders = (Shader**)realloc(shaders, nShaders*sizeof(Shader*));
    shaders[nShaders-1] = shader;
    glAttachShader(handle, shader->handle);
}

void Program::link()
{
    glLinkProgram(handle);
#ifdef DEBUG
    if(hasLinkError())
    {
        isLinked = false;
        printf("===== Program %s =====\n%s\n", name, linkError());
    } else
#endif //DEBUG
    isLinked = true;
}

void Program::use()
{
    glUseProgram(handle);
}

void Program::handleUniform(const char *uniformName)
{
    glUseProgram(handle);
    ++nUniforms;
    if(nUniforms == 0)
    {
        uniformNames = (char **) malloc(sizeof(char*));
        uniformLocations = (int *) malloc(sizeof(int));
    }
    else
    {
        uniformNames = (char **) realloc(uniformNames, nUniforms * sizeof(char*));
        uniformLocations = (int *) realloc(uniformLocations, nUniforms * sizeof(int));
    }
    uniformNames[nUniforms-1] = (char *) malloc(strlen(uniformName) + 3);
    sprintf(uniformNames[nUniforms-1], "%s", uniformName);
    uniformLocations[nUniforms-1] = glGetUniformLocation(handle, uniformName);
    glUseProgram(0);
}

int Program::uniformIndex(const char *uniformName) const
{
    for(int i=0; i<nUniforms; ++i)
    {
        if(!strcmp(uniformNames[i], uniformName)) return i;
    }
    return -1;
}

void Program::uniform1f(const char *uniformName, float value)
{
    glUniform1f(uniformLocations[uniformIndex(uniformName)], value);
}

void Program::uniform1i(const char *uniformName, int value)
{
    glUniform1i(uniformLocations[uniformIndex(uniformName)], value);
}

void Program::uniform2f(const char *uniformName, float value1, float value2)
{
    glUniform2f(uniformLocations[uniformIndex(uniformName)], value1, value2);
}

