#include <game.hpp>
#include <GLFW/glfw3.h>
#include <GLM/gtx/transform.hpp>
#include <terrain.hpp>
#include <entity.hpp>
#include <iostream>

Game::Game() : lastTime(0.0f), deltaTime(std::make_shared<float>(0.0f)), shaders(3),
	terrain(8.0f, 8.0f)
{
	initCrosshairVAO();
	player.camera.bindDeltaTime(deltaTime);

	shaders[SHADER::MODEL] = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
	shaders[SHADER::TERRAIN] = Shader("./shaders/terrain.vs", "./shaders/terrain.fs");
	shaders[SHADER::SKYBOX] = Shader("./shaders/skybox.vs", "./shaders/skybox.fs");

	Hitbox grimmHitbox(std::vector<glm::vec3>{
		{ -0.055279, -0.059106, 0.065943},
		{ 0.055279, -0.059106 , 0.065943 },
		{ 0.055279, 0.059106 , 0.065943 },
		{ -0.055279, 0.059106 , 0.065943 },

		{ -0.055279, -0.059106, -0.105943 },
		{ 0.055279, -0.059106, -0.105943 },
		{ 0.055279, 0.059106, -0.105943 },
		{ -0.055279, 0.059106, -0.105943 },
	});

	Model grimmModel("./assets/grimm/grimm.fbx");

	Enemy grimm(grimmModel, grimmHitbox, 10);
	grimm.scale(4.0f);
	//grimm.translate(glm::vec3(0.0f, 0.0f, 0.0f));
	grimm.rotate(glm::vec3(90.0f, 0.0f, 0.0f));

	enemies.push_back(
		grimm
	);
}

void Game::mainLoop()
{
	updateDeltaTime();

	auto playerTarget = Ray(player.camera, 10.0f);

	if (player.hasShot) {
		std::for_each(enemies.begin(), enemies.end(), [&](Enemy& enemy) {
			if (enemy.hitbox.testRayCollision(playerTarget)) {
				std::cout << "BOOM\n";
			}
			});

		player.hasShot = false;
	}

	drawEnemies();
	drawTerrain();
	drawCrosshair();
	drawSkybox();
}

void Game::updateDeltaTime()
{
	float currentTime = static_cast<float>(glfwGetTime());
	*deltaTime = currentTime - lastTime;
	lastTime = currentTime;
}

void Game::drawEnemies()
{
	std::for_each(enemies.begin(), enemies.end(), [&](Enemy& enemy) {
		enemy.draw(getShader(SHADER::MODEL), player.camera);
		});
}

void Game::drawTerrain()
{
	terrain.render(getShader(SHADER::MODEL), player.camera);
}

void Game::initCrosshairVAO()
{
	GLuint VBO = 0;

	glm::vec3 point(0.0f);

	glCreateVertexArrays(1, &crosshairVAO);
	glCreateBuffers(1, &VBO);

	glBindVertexArray(crosshairVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point), &point, GL_STATIC_DRAW);

	glVertexAttribPointer(Location::Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, reinterpret_cast<const void*>(sizeof(float) * 0));
	glEnableVertexAttribArray(Location::Position);

	glBindVertexArray(0);
}

void Game::drawCrosshair() {
	glBindVertexArray(crosshairVAO);
	Shader shader = getShader(SHADER::MODEL);
	glm::mat4 id = glm::mat4(1.0f);

	shader.setMat4("model", id);
	shader.setMat4("view", id);
	shader.setMat4("projection", id);

	glPointSize(3.0f);
	glDrawArrays(GL_POINTS, 0, 1);

	shader.reset();
	glBindVertexArray(0);
}

void Game::drawSkybox()
{
}
