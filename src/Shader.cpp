#include "Shader.h"

#include <cstring>

Shader::Shader(std::string vertexShader, std::string fragmentShader)
{
    // Lê os dois arquivos de shaders
    GLchar* vertexShaderSource = ReadFile(vertexShader);
    GLchar* fragmentShaderSource = ReadFile(fragmentShader);

    // Cria e compila ambos os shaders
    this->vs = CreateShader(vertexShaderSource, GL_VERTEX_SHADER);
    this->fs = CreateShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    // Cria o programa
    this->ProgramID = glCreateProgram();

    // Adiciona os dois shaders ao programa
    glAttachShader(this->ProgramID, this->vs);
    glAttachShader(this->ProgramID, this->fs);

    // Faz o linking do programa
    glLinkProgram(this->ProgramID);

    // Verifica se ocorreu algum erro
    GLint linked;
    glGetProgramiv(this->ProgramID, GL_LINK_STATUS, &linked);

    if(!linked)
    {
        // Retorna o tamanho da mensagem de erro
        GLsizei len;
        glGetProgramiv(this->ProgramID, GL_INFO_LOG_LENGTH, &len);

        // Aloca a string para ler o erro
        GLchar* log = new GLchar[len+1];
        glGetProgramInfoLog(this->ProgramID, len, &len, log);
        std::cerr << "Program linking error: " << log << std::endl;

        // Apaga os shader e o programa
        delete [] log;

        glDeleteShader(this->vs);
        glDeleteShader(this->fs);

        glDeleteProgram(this->ProgramID);
        this->ProgramID = 0;
    }

}

Shader::~Shader()
{
    // Desmonta o programa e apaga os shaders
    glDetachShader(this->ProgramID, this->vs);
    glDetachShader(this->ProgramID, this->fs);
    glDeleteShader(this->vs);
    glDeleteShader(this->fs);
    glDeleteProgram(this->ProgramID);
}

void Shader::UseShader()
{
    glUseProgram(this->ProgramID);
}

GLuint Shader::getUniform(std::string uniformName)
{
    return glGetUniformLocation(this->ProgramID, uniformName.c_str());
}

GLchar* Shader::ReadFile(std::string filePath)
{
    std::ifstream in( filePath.c_str(), std::ios::in | std::ios::binary );

    if(!in)
    {
        std::cerr << "Could not open shader file: " << filePath << std::endl;
        return NULL;
    }

    std::ostringstream content;
    content << in.rdbuf();

    in.close();

    std::string fileContent = content.str();

    char* source = new char[fileContent.length()+1];
    strcpy(source, fileContent.c_str());
    source[fileContent.length()] = '\0';
    //std::cout << filePath << " read successfuly" << std::endl;
    return source;
}

GLuint Shader::CreateShader(GLchar* sourceCode, GLenum shaderType)
{
    // Retorna um ponteiro para a string contendo o codigo fonte do shader
    const GLchar* sourcePointer = sourceCode;

    // Cria o shader, adiciona o codigo fonte e compila
    GLuint subShaderID = glCreateShader(shaderType);
    glShaderSource(subShaderID, 1, &sourcePointer, NULL);
    glCompileShader(subShaderID);

    // Checa por algum erro durante a compilação
    GLint compiled;
    glGetShaderiv(subShaderID, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        // Retorna o tamanho da mensagem de erro
        GLsizei len;
        glGetShaderiv(subShaderID, GL_INFO_LOG_LENGTH, &len);

        // Aloca e lê o erro
        GLchar* log = new GLchar[len+1];
        glGetShaderInfoLog(subShaderID, len, &len, log);

        if(shaderType == GL_VERTEX_SHADER)
            std::cerr << "Vertex ";
        else if(shaderType == GL_FRAGMENT_SHADER)
            std::cerr << "Fragment ";

        // Imprime o erro de compilação
        std::cerr << "Shader compile error: " << log << std::endl;
        delete [] log;
        return 0;
    }

    // Deleta o codigo fonte do shader da memoria
    delete [] sourceCode;
    sourceCode = NULL;

    // Retorna o sahder criado
    return subShaderID;
}
