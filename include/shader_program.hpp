#ifndef SHADER_HPP
#define SHADER_HPP

#include <GLM/mat4x4.hpp>

class ShaderProgram 
{
public:
    ShaderProgram(const char* vertexFilename, const char* fragmentFilename);
    GLuint compileShader(const char *fileName, GLuint &shaderID);
    GLuint getProgramID() const { return programID; };

    void setMat4(const char* uniformName, glm::mat4 &matrix);

private:
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    GLuint programID;
};

#endif