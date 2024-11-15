#ifndef SHADER_HPP
#define SHADER_HPP

#include <GLM/mat4x4.hpp>

class ShaderProgram 
{
public:
    ShaderProgram(const char* vertexFilename, const char* fragmentFilename);
    GLuint compileShader(const char *fileName, GLuint &shaderID);
    GLuint getProgramID() const { return programID; };

    void use() const;

    void setVec3(const char* uniformName, glm::vec3 v) const;
    void setMat4(const char* uniformName, glm::mat4 &matrix) const;
    void setFloat(const char* uniformName, float value) const;

private:
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    GLuint programID;
};

#endif