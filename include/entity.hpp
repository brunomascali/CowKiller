#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <camera.hpp>
#include <model.hpp>
#include <GLM/trigonometric.hpp>
#include <bezier.hpp>
#include <collision.hpp>
#include <memory>
#include <GLM/gtx/string_cast.hpp>

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
	char hp;
	std::unique_ptr<Collider> collider;
	std::optional<Bezier> bezier;

	glm::vec3 position;
	glm::vec3 rotation;
	float scaling;

	Enemy(Model model, char hp) : model(model), hp(hp), collider(nullptr) {}

	Enemy(const Enemy& other) noexcept
		: model(std::move(other.model)),
		hp(other.hp),
		bezier(other.bezier),
		position(other.position),
		rotation(other.rotation),
		scaling(other.scaling) {

		if (other.collider)
			this->collider = other.collider->clone();
	}

	Enemy& operator=(Enemy& other) noexcept {
		if (this != &other) {
			model = std::move(other.model);
			hp = other.hp;
			collider = std::move(other.collider);
			bezier = std::move(other.bezier);
			position = other.position;
			rotation = other.rotation;
			scaling = other.scaling;
		}
		return *this;
	}

	void setPosition(glm::vec3 newPosition) {
		glm::vec3 translation = newPosition - position;
		this->position = newPosition;

		if (collider) {
			collider->translate(translation);
		}
	}
	
	void setBezierCurve(Bezier& bezier) {
		this->bezier = std::optional<Bezier>{bezier};
	}

	void translate(glm::vec3 t) {
		position += t;

		if (collider) {
			collider->translate(t);
		}
	}

	void rotate(glm::vec3 angles) {
		rotation.x += glm::radians(angles.x);
		rotation.y += glm::radians(angles.y);
		rotation.z += glm::radians(angles.z);

		if (collider)
			collider->rotate(angles, position);
	}

	void scale(float s) {
		scaling = s;
		if (collider)
			collider->scale(s, position);
	}

	void render(Shader& shader, CameraFree& camera) {
		model.render(shader, camera, position, rotation, scaling);

		if (collider) {
			collider->render(shader, camera, position, rotation, scaling);
		}
	}
};

#endif