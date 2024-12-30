#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <vector>
#include <terrain_block.hpp>
#include <texture.hpp>
#include <shader.hpp>
#include <camera.hpp>
#include "GLM/gtc/noise.hpp"

class Terrain {
public:
	Terrain(const float terrainWidth, const float terrainDepth) {
		const float blockWidth = static_cast<float>(terrainWidth) / numBlocksX;
		const float blockDepth = static_cast<float>(terrainDepth) / numBlocksZ;

		diffusion = Texture("./assets/terrain/diffuse.jpg");
		ambientOcclusion = Texture("./assets/terrain/ambient_occlusion.jpg");
		displacement = Texture("./assets/terrain/displacement.jpg");

		for (size_t z = 0; z < numBlocksZ; z++) {
			for (size_t x = 0; x < numBlocksX; x++) {
				blocks.emplace_back(x * blockWidth, z * blockDepth, terrainWidth, terrainDepth);
			}
		}
	}

	float getHeight(glm::vec2 position) {
		return glm::perlin(position / PERLINSCALINGFACTOR);
	}

	void render(Shader& shader, CameraFree& camera) const {
		glm::mat4 v = camera.getViewMatrix();
		glm::mat4 p = camera.getProjectionMatrix();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffusion.id);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ambientOcclusion.id);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, displacement.id);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughness.id);

		shader.use();
		shader.setMat4("view", v);
		shader.setMat4("projection", p);

		shader.setInt("diffusion", 0);
		shader.setInt("ao", 1);
		shader.setInt("displacement", 2);
		shader.setInt("roughness", 3);

		for (const auto& block : this->blocks)
			block.render();

		shader.unbind();
	}
private:
	float terrainWidth;
	float terrainDepth;

	Texture diffusion;
	Texture ambientOcclusion;
	Texture displacement;
	Texture roughness;

	std::vector<TerrainBlock> blocks;
};

#endif 