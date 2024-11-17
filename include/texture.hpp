#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <assimp/scene.h>
#include <filesystem>

class Texture {
public:
	Texture(const aiTextureType type, const std::string& modelName, const std::string& textureFilename);

	GLuint id;
	aiTextureType type;
	std::filesystem::path path;
};

#endif