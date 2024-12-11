#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <vector>
#include <terrain_block.hpp>
#include <texture.hpp>
#include <shader.hpp>
#include <camera.hpp>

constexpr int numBlocksX = 8;
constexpr int numBlocksZ = 8;
constexpr float VERTEX_SPACING = 1.0f / 8.0f;
constexpr float perlinScalingFactor = 1.0f / 8.0f;

class Terrain {
public:
	Terrain(const float terrainWidth, const float terrainDepth);

	float getHeight(glm::vec2 position);

	void render(Shader &shader, CameraFree& camera) const;
private:
	float terrainWidth;
	float terrainDepth;

	Texture texture;

	std::vector<TerrainBlock> blocks;
};

#endif 