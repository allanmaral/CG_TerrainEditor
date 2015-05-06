#include "Texture.h"

Texture::Texture(std::string texturePath)
{
    // Primeiro V� qual � a exten��o da imagem
    if(texturePath.substr(texturePath.find_last_of(".")+1) == "bmp")
    {
        readBMPFile(texturePath);
    }
    // Possivelment sera adicionado suporte a outros formatos
}
Texture::~Texture()
{
    glDeleteTextures(1, &(this->TextureID));
}
void Texture::UseTexture()
{
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
}
void Texture::readBMPFile(std::string texturePath)
{
    unsigned char header[54];   // Cabe�alho do arquivo BMP
    unsigned int dataPos;       // Posi��o onde come�am os dados de pixel da imagem
    unsigned int width, height; // Largura e altura da imagem
    unsigned int imageSize;     // Tamanho, em bytes, da imagem

    unsigned char *data;        // Dados dos pixels

    std::ifstream in(texturePath.c_str(), std::ios::in | std::ios::binary);
    if(!in)
    {
        std::cerr << "Could not open image: " << texturePath << std::endl;
        return;
    }

    // Le o cabe�alho da imagem
    in.read((char*)header, 54);

    // Todos os arquivos BMP tem seus primeiro dois bytes como B e M
    if(header[0] != 'B' || header[1] != 'M')
    {
        std::cerr << "Not correct BMP file format" << std::endl;
        std::cerr << (int)header[1] << std::endl;
        return;
    }

    // Passa os dados do cabe�alho
    dataPos     = *(int*)&(header[0x0A]);
    imageSize   = *(int*)&(header[0x22]);
    width       = *(int*)&(header[0x12]);
    height      = *(int*)&(header[0x16]);

    // Alguns arquivos podem ter os dados mal formatados
    if(imageSize ==0)   imageSize=width*height*3; //Tamanho padr�o 3 por pixel
    if(dataPos==0)  dataPos=54; // Se n�o tiver nada, assume que � logo depois do cabe�alho

    // Le o restante do arquivo
    data = new unsigned char [imageSize];
    in.read((char*)data, imageSize);
    in.close();

    // Cria a texutra do OpenGL
    glGenTextures(1, &(this->TextureID));
    glBindTexture(GL_TEXTURE_2D, this->TextureID);

    // Passa os dados para o OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // Define os filtros de amplia��o e diminui��o
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    delete [] data;
}
