#ifndef SHADER_HPP
#define SHADER_HPP

#include <GLM/mat4x4.hpp>
#include <glad/glad.h>

enum SHADER {
	TERRAIN = 0,
	MODEL = 1,
	SKYBOX = 2,
};

class Shader
{
public:
	Shader() {};
	Shader(const char* vertexFilename, const char* fragmentFilename);
	GLuint compileShader(const char* fileName, GLuint& shaderID);
	GLuint getProgramID() const { return programID; };

	void use() const;
	static void reset();

	void setVec3(const char* uniformName, glm::vec3 v) const;
	void setMat4(const char* uniformName, glm::mat4& matrix) const;
	void setFloat(const char* uniformName, float value) const;
	void setInt(const char* uniformName, int value) const;

private:
	GLuint vertexShaderID = 0;
	GLuint fragmentShaderID = 0;
	GLuint programID = 0;
};

#endif