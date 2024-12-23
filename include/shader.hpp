#ifndef SHADER_HPP
#define SHADER_HPP

#include <GLM/mat4x4.hpp>
#include <glad/glad.h>
#include <vector>
#include <fstream>
#include <GLM/gtc/type_ptr.hpp>

enum SHADER {
	TERRAIN = 0,
	MODEL = 1,
	SKYBOX = 2,
};

class Shader
{
public:
	Shader() {};
	Shader(const char* vertexFilename, const char* fragmentFilename) {
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

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
			printf("Erro ao linkar shaders: %s\n", buffer);
			exit(1);
		}
	}

	GLuint compileShader(const char* fileName, GLuint& shaderID) {
		std::string shaderCode;
		std::ifstream shaderStream(fileName, std::ios::in);
		if (shaderStream.is_open()) {
			std::stringstream sstr;
			sstr << shaderStream.rdbuf();
			shaderCode = sstr.str();
			shaderStream.close();
		}
		else {
			printf("Erro ao abrir arquivo %s\n", fileName);
			exit(1);
		}

		const char* shaderCodePtr = shaderCode.c_str();
		glShaderSource(shaderID, 1, &shaderCodePtr, nullptr);
		glCompileShader(shaderID);

		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
			printf("Erro ao compilar shader %s: %s\n", fileName, infoLog);
			exit(1);
		}

		return shaderID;
	}

	GLuint getProgramID() const {
		return programID;
	};

	void use() const {
		glUseProgram(programID);

	}

	static void reset() {
		glUseProgram(0);
	}

	void setVec3(const char* uniformName, glm::vec3 v) const
	{
		GLint uniform = glGetUniformLocation(this->getProgramID(), uniformName);
		glUniform3fv(uniform, 1, glm::value_ptr(v));
	}

	void setMat4(const char* uniformName, glm::mat4& matrix) const
	{
		GLint uniform = glGetUniformLocation(this->getProgramID(), uniformName);
		glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void setFloat(const char* uniformName, float value) const
	{
		GLint uniform = glGetUniformLocation(this->getProgramID(), uniformName);
		glUniform1f(uniform, value);
	}

	void setInt(const char* uniformName, int value) const
	{
		GLint uniform = glGetUniformLocation(this->getProgramID(), uniformName);
		glUniform1i(uniform, value);
	}

	void setMat4Arr(const char* uniformName, std::vector<glm::mat4>& v) const
	{
		std::vector<float> flattenedBoneMatrices;
		for (const glm::mat4& matrix : v) {
			const float* ptr = glm::value_ptr(matrix);
			flattenedBoneMatrices.insert(flattenedBoneMatrices.end(), ptr, ptr + 16);
		}

		GLuint uniform = glGetUniformLocation(this->getProgramID(), uniformName);
		glUniformMatrix4fv(uniform, static_cast<GLsizei>(v.size()), GL_FALSE, flattenedBoneMatrices.data());
	}


private:
	GLuint programID = 0;
};

#endif