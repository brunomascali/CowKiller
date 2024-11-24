#include <iostream>
#include <string>
#include <format>
#include <filesystem>

#include <model.hpp>
#include <texture.hpp>
#include <stb_image.h>

Texture::Texture(const std::string& modelName, const std::string& textureFilename)
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

	GLenum format = 0;
	if (numChannels == 1)
		format = GL_RED;
	else if (numChannels == 3)
		format = GL_RGB;
	else
		format = GL_RGBA;

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

Texture::Texture(const std::string& texturePath)
{
	int width, depth, numChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &depth, &numChannels, 0);
	if (!data)
	{
		std::cerr << "Textura não encontrada: " << path << std::endl;
		std::exit(-1);
	}

	GLenum format = 0;
	if (numChannels == 1)
		format = GL_RED;
	else if (numChannels == 3)
		format = GL_RGB;
	else
		format = GL_RGBA;

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
