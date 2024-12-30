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
	Game() : terrain(8.0f, 8.0f) {
		lastTime = 0.0f;
		deltaTime = std::make_shared<float>(0.0f);
		shaders = std::vector<Shader>(3);

		player.camera.bindDeltaTime(deltaTime);

		shaders[SHADER::MODEL] = Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
		shaders[SHADER::TERRAIN] = Shader("./shaders/terrain.vs", "./shaders/terrain.fs");
		shaders[SHADER::SKYBOX] = Shader("./shaders/skybox.vs", "./shaders/skybox.fs");

		// Modelo da arma do personagem
		Model gunModel("./assets/gun/guntest.fbx");
		gunModel.scaling = 4.0f;
		Texture gunAmbient("./assets/gun/texture/Plasmagun_a.png");
		Texture gunRoughness("./assets/gun/texture/Plasmagun_r.png");
		Texture gunAO("./assets/gun/texture/Plasmagun_ao.png");
		Texture gunEmissive("./assets/gun/texture/Plasmagun_e.png");
		gunModel.addTexture(TextureType::AMBIENT, gunAmbient, 0);
		gunModel.addTexture(TextureType::AO, gunAO, 0);
		gunModel.addTexture(TextureType::ROUGHNESS, gunRoughness, 0);
		gunModel.addTexture(TextureType::EMISSIVE, gunEmissive, 0);
		models.push_back(gunModel);

		// Modelo do dragão
		Model dragonModel("./assets/dragon/dragon.fbx");
		Texture dragonAmbient("./assets/dragon/dragon.png");
		dragonModel.addTexture(TextureType::AMBIENT, dragonAmbient, 0);
		Enemy dragon(dragonModel, 10);
		dragon.scale(0.0005f);
		dragon.translate({ 5.0f, 2.0f, 5.0f });

		//Modelo de uma esfera
		Model sphere("./assets/sphere/sphere.fbx");
		models.push_back(sphere);

		Hitbox grimmHitbox(std::vector<glm::vec3>{
			{ -0.055279, -0.059106, 0.065943 },
			{ 0.055279, -0.059106 , 0.065943 },
			{ 0.055279, 0.059106 , 0.065943 },
			{ -0.055279, 0.059106 , 0.065943 },

			{ -0.055279, -0.059106, -0.105943 },
			{ 0.055279, -0.059106, -0.105943 },
			{ 0.055279, 0.059106, -0.105943 },
			{ -0.055279, 0.059106, -0.105943 },
		});

		Model grimmModel("./assets/grimm/baby.fbx");
		Texture grimmAmbient("./assets/grimm/ambient.png");
		grimmModel.addTexture(TextureType::AMBIENT, grimmAmbient, 0);

		Enemy grimm(grimmModel, 10);
		grimm.collider = grimmHitbox.clone();
		// Essa ordem precisa ser preservada
		grimm.rotate(glm::vec3(90.0f, 0.0f, 0.0f));
		grimm.scale(4.0f);
		grimm.translate(glm::vec3(5.0f, 1.0f, 5.0f));

		Enemy grimm2(grimmModel, 10);
		grimm2.collider = grimmHitbox.clone();
		// Essa ordem precisa ser preservada
		grimm2.rotate(glm::vec3(90.0f, 0.0f, 0.0f));
		grimm2.scale(4.0f);
		grimm2.translate(glm::vec3(7.0f, 1.0f, 5.0f));

		// Bezier
		Bezier grimmBezier({ 0.0f, 2.0f, 0.0f }, { 2.0, 2.0f, 0.0f }, { 2.0f, 2.0f, 2.0f }, { 4.0f, 2.0f, 2.0f }, 4.0);
		grimm.setBezierCurve(grimmBezier);

		enemies.push_back(grimm);
		enemies.push_back(grimm2);
		enemies.push_back(dragon);
	}

	void updateDeltaTime() {
		float currentTime = static_cast<float>(glfwGetTime());
		*deltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}

	void mainLoop() {
		updateDeltaTime();
		player.camera.updateViewMatrix();

		const Ray playerTarget(player.camera, 10.0f);

		for (Enemy& enemy : enemies) {
			if (enemy.bezier.has_value()) {
				enemy.setPosition(enemy.bezier.value().getPoint(lastTime));
			}
			if (player.hasShot && enemy.collider && enemy.collider->intersectRay(playerTarget)) {
				std::cout << "Boom\n";
			}
		}

		// drawSkybox();
		drawTerrain();
		drawEnemies();
		drawModels();
		drawCrosshair();
		player.hasShot = false;
	}

	Shader& getShader(SHADER id) {
		return shaders[id];
	};

	void drawEnemies() {
		std::for_each(enemies.begin(), enemies.end(), [&](Enemy& enemy) {
			enemy.render(getShader(SHADER::MODEL), player.camera);
			});
	}

	void drawTerrain() {
		terrain.render(getShader(SHADER::TERRAIN), player.camera);
	}

	void drawModels() {
		for (auto& model : models) {
			glm::mat4 v = player.camera.getViewMatrix();
			glm::mat4 p = player.camera.getProjectionMatrix();

			Shader& shader = getShader(SHADER::MODEL);

			shader.use();
			shader.setMat4("view", v);
			shader.setMat4("projection", p);

			if (model.modelName == "guntest") {
				glClear(GL_DEPTH_BUFFER_BIT);
				shader.setVec3("rotation", glm::vec3(-glm::radians(player.camera.phi + 270.0f), glm::radians(player.camera.theta + 90.0f), glm::radians(0.0f)));
				shader.setVec3("translation", player.camera.position - 0.25f * player.camera.forward + 0.4f * player.camera.right - 0.45f * player.camera.up);
			}
			else {
				shader.setVec3("rotation", model.rotation);
				shader.setVec3("translation", model.position);
			}
			shader.setFloat("scaling", model.scaling);

			shader.setInt("textureFlags", model.textureFlags);
			shader.setInt("textureAmbient", 0);
			shader.setInt("textureNormal", 1);
			shader.setInt("textureAO", 2);

			for (auto& mesh : model.meshes) {
				mesh.render(shader);
			}

			shader.unbind();
		}
	}

	void drawSkybox() {
		skybox.render(getShader(SHADER::SKYBOX), player.camera);
	}

	void initCrosshairVAO() {
		float crosshairVertices[] = {
			-0.01f, -0.01f, 0.0f,
			0.01f, -0.01f, 0.0f,
			0.01f, 0.01f, 0.0f,
			-0.01f, 0.01f, 0.0f
		};

		GLuint crosshairIndices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLuint crosshairVBO, crosshairEBO;

		glGenVertexArrays(1, &crosshairVAO);
		glGenBuffers(1, &crosshairVBO);
		glGenBuffers(1, &crosshairEBO);

		glBindVertexArray(crosshairVAO);

		glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, crosshairEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(crosshairIndices), crosshairIndices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void drawCrosshair() {
		glBindVertexArray(crosshairVAO);
		Shader shader = getShader(SHADER::MODEL);
		glm::mat4 id = glm::mat4(1.0f);

		shader.setMat4("model", id);
		shader.setMat4("view", id);
		shader.setMat4("projection", id);

		glPointSize(3.0f);
		glDrawArrays(GL_POINTS, 0, 1);

		shader.unbind();
		glBindVertexArray(0);
	}

	Player player;
	std::vector<Enemy> enemies;
	std::vector<Model> models;
	Terrain terrain;
	Skybox skybox;

	float lastTime;
	std::shared_ptr<float> deltaTime;

	std::vector<Shader> shaders;
	GLuint crosshairVAO;
};

#endif