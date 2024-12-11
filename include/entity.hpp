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
    glm::vec3 position;
    glm::vec3 rotation;
    float scaling;

	Enemy(Model model, Hitbox hitbox, char hp) : model(model), hitbox(hitbox), hp(hp), position(0.0f), rotation(0.0f), scaling(1.0f) {}

	void translate(glm::vec3 t);
	void rotate(glm::vec3 angles);
	void scale(float s);

	void draw(Shader& shader, CameraFree& camera);
};

#endif