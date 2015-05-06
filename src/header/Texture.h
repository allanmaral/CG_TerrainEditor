#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <iostream>

class Texture
{
public:
    Texture(std::string texturePath);
    ~Texture();
    void UseTexture();

private:
    GLuint TextureID;
    void readBMPFile(std::string texturePath);

};

#endif // TEXTURE_H
