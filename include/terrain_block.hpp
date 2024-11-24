#ifndef TERRAIN_BLOCK_HPP
#define TERRAIN_BLOCK_HPP

#include <cstdint>
#include <GLM/vec2.hpp>
#include <glad/glad.h>

// Para fazer um bloco NxN é necessário (N+1)² vértices, por isso VertexBlockCount é ímpar
constexpr uint32_t BLOCK_VERTICES_SIZE = 9;
constexpr float BLOCK_RESOLUTION = 0.125f;
constexpr float BLOCK_LENGTH = (BLOCK_VERTICES_SIZE - 1) * BLOCK_RESOLUTION;

class TerrainBlock {
public:
	// (x, z) estão em coordenadas do terreno global
	TerrainBlock(float posX, float posZ, float worldWidth, float worldDepth);
	void render() const;

private:
	glm::vec2 blockPosition;

	GLuint VAO;
	unsigned int vertexCount;
	bool visible = true;
};


#endif