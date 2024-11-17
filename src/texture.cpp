#include <iostream>
#include <string>
#include <format>
#include <filesystem>

#include <model.hpp>
#include <texture.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const aiTextureType type, const std::string& modelName, const std::string& textureFilename)
{
	this->path = std::format("./assets/{}/{}", modelName, textureFilename);
	glGenTextures(1, &id);

	int width, height, numChannels;
	unsigned char* data = stbi_load(this->path.string().c_str(), &width, &height, &numChannels, 0);
	if (!data)
	{
		std::cerr << "Textura não encontrada: " << path << std::endl;
		std::exit(-1);
	}

	GLenum format = (numChannels == 4) ? GL_RGBA : GL_RGB;

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
