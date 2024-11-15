#include <format>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glad/glad.h>
#include <GLM/mat4x4.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <shader_program.hpp>

static void checkShaderCompileErrors(GLuint shader, const std::string &type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            throw std::runtime_error(type + " SHADER COMPILATION ERROR\n" + infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            throw std::runtime_error(infoLog);
        }
    }
}

ShaderProgram::ShaderProgram(const char *vertexFilename, const char *fragmentFilename)
{
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    compileShader(vertexFilename, vertexShaderID);
    checkShaderCompileErrors(vertexShaderID, "VERTEX");
    compileShader(fragmentFilename, fragmentShaderID);
    checkShaderCompileErrors(fragmentShaderID, "FRAGMENT");

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
    try
    {
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

    const GLchar *shader = shaderString.c_str();
    const GLint length = static_cast<GLint>(shaderString.length());

    glShaderSource(shaderID, 1, &shader, &length);
    glCompileShader(shaderID);

    return shaderID;
}

void ShaderProgram::use() const
{
    glUseProgram(this->getProgramID());
}

void ShaderProgram::setVec3(const char* uniformName, glm::vec3 v) const
{
    GLint uniform = glGetUniformLocation(this->getProgramID(), uniformName);
    glUniform3fv(uniform, 1, glm::value_ptr(v));
}

void ShaderProgram::setMat4(const char *uniformName, glm::mat4 &matrix) const
{
    GLint uniform = glGetUniformLocation(this->getProgramID(), uniformName);
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setFloat(const char* uniformName, float value) const
{
    GLint uniform = glGetUniformLocation(this->getProgramID(), uniformName);
    glUniform1f(uniform, value);
}
