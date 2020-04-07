#include "Shader.hpp"
#include "OpenGL.hpp"

Shader::Shader(const char* source)
    : isCompiled(false)
{
    handle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(handle, 1, (const GLchar**)&source, NULL);
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
