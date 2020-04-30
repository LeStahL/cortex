#include "Texture.hpp"

#include <cstdio>

Texture::Texture(const char *_name, int _width, int _height)
    : width(_width)
    , height(_height)
{
    if(_name == nullptr)
    {
        name = (char *) malloc(24);
        sprintf(name, "Unnamed_%d", ++TEXTURE_ID);
    }
    else 
    {
        name = (char *) malloc(strlen(name) + 3);
        sprintf(name, "%s", _name);
    }

    data = malloc(width * height * 4);

    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, handle);
}