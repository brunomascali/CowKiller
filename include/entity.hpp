#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <camera.hpp>
#include <model.hpp>
#include <collision.hpp>
#include <GLM/trigonometric.hpp>

class Player {
public:
	CameraFree camera;
	bool hasShot;

	Player() : camera(CameraFree(glm::vec3(2.0f, 0.0f, 2.0f))) {}
	Player(glm::vec3 position) : camera(CameraFree(position)) {}
};

class Enemy {
public:
	Model model;
	Hitbox hitbox;
	char hp;

	glm::vec3 translation;
	glm::vec3 rotation;
	float scaling;

	Enemy(Model model, Hitbox hitbox, char hp) : model(model), hitbox(hitbox), hp(hp) {
	}
	Enemy(std::string modelPath, std::vector<glm::vec3> hitboxPoints, char hp, glm::vec3 t, glm::vec3 r, float s) : model(model), hitbox(hitbox), hp(hp) {
		model = Model(modelPath);
		hitbox = Hitbox(hitboxPoints);
		translate(t);
		rotate(r);
		scale(s);
	}

	void translate(glm::vec3 t) {
		translation += t;
		for (auto& plane : hitbox.planes) {
			plane.translate(t);
		}
	}

	void rotate(glm::vec3 angles) {
		rotation.x += glm::radians(angles.x);
		rotation.y += glm::radians(angles.y);
		rotation.z += glm::radians(angles.z);

		for (auto& plane : hitbox.planes) {
			plane.rotate(angles, translation);
		}
	}

	void scale(float s) {
		scaling = s;
		for (auto& plane : hitbox.planes) {
			plane.scale(s, translation);
		}
	}

	void render(Shader& shader, CameraFree& camera) {
		model.render(shader, camera, translation, rotation, scaling);

		// Descomentar se precisar ver a hitbox
		//for (auto& plane : hitbox.planes)
		//	plane.render(shader, camera);
	}
};

#endif