#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "GL/glew.h"

class Shader
{
public:
    Shader(std::string vertexShader, std::string fragmentShader);
    ~Shader();
    void UseShader();

private:
    GLuint ProgramID;
    GLuint vs, fs;
    GLchar* ReadFile(std::string filePath);
    GLuint CreateShader(GLchar* sourceCode, GLenum shaderType);

};

#endif // SHADER_H
