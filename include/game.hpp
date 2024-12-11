#ifndef GAME_HPP
#define GAME_HPP

#include <model.hpp>
#include <collision.hpp>
#include <GLM/trigonometric.hpp>
#include <terrain.hpp>
#include <entity.hpp>
#include <skybox.hpp>

class Game {
public:
	Game();

	void mainLoop();

	void updateDeltaTime();
	void addEnemy(Model model, Hitbox hitbox, char hp) { enemies.emplace_back(model, hitbox, hp); }

	Shader& getShader(SHADER id) { 
		return shaders[id]; 
	};

	void drawEnemies();
	void drawTerrain();
	void initCrosshairVAO();
	void drawCrosshair();
	void drawSkybox();

	Player player;
	std::vector<Enemy> enemies;
	Terrain terrain;
	Skybox skybox;
private:
	float lastTime;
	std::shared_ptr<float> deltaTime;
	std::vector<Shader> shaders;

	GLuint crosshairVAO;
};

#endif