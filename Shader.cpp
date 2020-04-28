#include "Shader.hpp"
#include "OpenGL.hpp"

#include <cstdio>

Shader::Shader(const char* _source, const char *_symbol)
    : isCompiled(false)
    , isSymbol(_symbol != nullptr)
{
    handle = glCreateShader(GL_FRAGMENT_SHADER);
    int size = strlen(_source);
    glShaderSource(handle, 1, (const GLchar**)&_source, &size);
    
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
    return compileStatus != GL_TRUE;
}

char *Shader::compileError()
{
    int infoLogLength;
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *infoLog = (GLchar *) malloc(infoLogLength*sizeof(GLchar));
    glGetShaderInfoLog(handle, infoLogLength, NULL, infoLog);
    return infoLog;
}
#endif // DEBUG

void Shader::compile()
{
    glCompileShader(handle);
#ifdef DEBUG
    if(hasCompileError())
    {
        isCompiled = false;
        printf("===== %s =====\n%s\n", symbol, compileError());
    } else
#endif
    isCompiled = true;
}
