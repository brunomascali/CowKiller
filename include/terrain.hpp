#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <vector>
#include <terrain_block.hpp>
#include <texture.hpp>
#include <shader.hpp>
#include <camera.hpp>

class Terrain {
public:
	Terrain(uint32_t numBlocksX, uint32_t numBlocksZ);

	void render(Shader &shader, CameraFree& camera) const;
	
	std::vector<std::vector<float>> height;
private:
	Texture heightmapTexture;
	Texture normalsTexture;
	std::vector<TerrainBlock> blocks;
};

#endif 