#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <glad/glad.h>
#include <texture.hpp>
#include <shader.hpp>
#include <camera.hpp>

class Skybox {
public:
	Skybox();

	void render(Shader& shader, CameraFree& camera) const;

private:
	GLuint VAO;
	Texture texture;
};

#endif