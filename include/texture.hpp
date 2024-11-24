#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <assimp/scene.h>
#include <filesystem>

class Texture {
public:
	// Para criar texturas a partir de um mesh do assimp
	Texture(const std::string& modelName, const std::string& texturePath);
	// Criar texturas genéricas
	explicit Texture(const std::string& texturePath);

	GLuint id;
	aiTextureType type;
	std::filesystem::path path;
};

#endif