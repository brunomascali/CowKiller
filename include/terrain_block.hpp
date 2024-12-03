#ifndef TERRAIN_BLOCK_HPP
#define TERRAIN_BLOCK_HPP

#include <camera.hpp>
#include <cstdint>
#include <vector>
#include <GLM/vec2.hpp>
#include <glad/glad.h>

class TerrainBlock {
public:
	// (x, z) estão em coordenadas do terreno global
	TerrainBlock(float posX, float posZ, float worldWidth, float worldDepth);

	void render() const;

	bool isVisible(CameraFree &camera) const;

private:
	GLuint VAO;
	GLsizei vertexCount;
	glm::vec2 blockPosition;
};


#endif