#include "Shader.hpp"
#include "OpenGL.hpp"

#include <cstdio>

Shader::Shader(const char* _source, const char *_symbol)
    : isCompiled(false)
    , isSymbol(_symbol != nullptr)
{
    handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(handle, 1, (const GLchar**)&_source, NULL);
    
    if(_symbol != nullptr)
    {
        symbol = (char *) malloc(strlen(_symbol) + 3);
        sprintf(symbol, "%s", _symbol);
    }
}

#ifdef DEBUG
bool Shader::hasCompileError()
{
    GLint compileStatus;

    glGetShaderiv(handle, GL_COMPILE_STATUS, &compileStatus);
    return compileStatus == GL_TRUE;
}

char *Shader::compileError()
{
    int infoLogLenght;
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLenght);
    char *infoLog = (char *) malloc(infoLogLenght);
    glGetShaderInfoLog(handle, infoLogLenght, NULL, infoLog);
    return infoLog;
}
#endif // DEBUG

void Shader::compile()
{
    glCompileShader(handle);
#ifdef DEBUG
    if(!hasCompileError())
#endif
    isCompiled = true;
}
