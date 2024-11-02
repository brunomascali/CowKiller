#include <format>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glad.h>
#include <GLM/mat4x4.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <shader_program.hpp>

ShaderProgram::ShaderProgram(const char *vertexFilename, const char *fragmentFilename)
{
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    compileShader(vertexFilename, vertexShaderID);
    compileShader(fragmentFilename, fragmentShaderID);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    GLint link_ok;
    glGetProgramiv(programID, GL_LINK_STATUS, &link_ok);
    if (link_ok == GL_FALSE)
    {
        char buffer[256];
        glGetProgramInfoLog(programID, 256, nullptr, buffer);
        throw std::runtime_error(std::format("Erro ao linkar shaders: {}\n", buffer));
    }
}

GLuint ShaderProgram::compileShader(const char *fileName, GLuint &shaderID)
{
    std::ifstream file;
    try {
        file.exceptions(std::ifstream::failbit);
        file.open(fileName);
    }
    catch (std::exception &e)
    {
        throw std::runtime_error(std::format("Erro {} ao abrir o shader {}", e.what(), fileName));
    }

    std::stringstream temp;
    temp << file.rdbuf();
    std::string shaderString = temp.str();

    const GLchar* shader = shaderString.c_str();
    const GLint length = static_cast<GLint>(shaderString.length());

    glShaderSource(shaderID, 1, &shader, &length);
    glCompileShader(shaderID);
    
    return shaderID;
}

void ShaderProgram::setMat4(const char* uniformName, glm::mat4 &matrix)
{
    GLint uniform = glGetUniformLocation(this->getProgramID(), uniformName);
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
}
