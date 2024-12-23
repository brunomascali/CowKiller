#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <iostream>
#include <glad/glad.h>
#include <assimp/scene.h>
#include <filesystem>
#include <stb_image.h>

enum TextureType {
	AMBIENT,
	NORMAL,
	AO,
	ROUGHNESS,
	EMISSIVE,
	TEXTURETYPECOUNT
};

class Texture {
public:
	// Para criar texturas a partir de um mesh do assimp
	Texture() : id(0), type(aiTextureType_NONE), path("") {};

	Texture(const std::string& modelName, const std::string& texturePath) {
		this->path = std::format("./assets/{}/{}", modelName, texturePath);
		glGenTextures(1, &id);

		int width, height, numChannels;
		unsigned char* data = stbi_load(this->path.string().c_str(), &width, &height, &numChannels, 0);
		if (!data)
		{
			std::cerr << "Textura não encontrada: " << path << std::endl;
			return;
			//std::exit(-1);
		}

		GLenum format = getFormat(numChannels);

		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}

	// Criar texturas genéricas
	explicit Texture(const std::string& texturePath) {
		int width, depth, numChannels;
		unsigned char* data = stbi_load(texturePath.c_str(), &width, &depth, &numChannels, 0);
		if (!data)
		{
			std::cerr << "Textura não encontrada: " << path << std::endl;
			std::exit(-1);
		}

		GLenum format = getFormat(numChannels);

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, depth, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}

	// Criar textura para skybox
	Texture(std::vector<std::string> skyboxTextureFiles) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);

		int width, depth, numChannels;
		for (unsigned int i = 0; i < 6; i++) {
			std::string path = "./assets/skybox/" + skyboxTextureFiles[i];
			unsigned char* data = stbi_load(path.c_str(), &width, &depth, &numChannels, 0);

			if (data == nullptr) {
				throw std::runtime_error("Erro ao carregar textura");
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, depth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	static GLenum getFormat(int numChannels) {
		if (numChannels == 1)
			return GL_RED;
		else if (numChannels == 3)
			return GL_RGB;

		return GL_RGBA;
	}

	GLuint id;
	aiTextureType type;
	std::filesystem::path path;
};

#endif