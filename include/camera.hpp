#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <memory>
#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>
#include <linalg.hpp>
#include <GLFW/glfw3.h>
#include <terrain_block.hpp>

constexpr glm::vec3 GLOBAL_UP = glm::vec3(0.0f, 1.0f, 0.0f);
constexpr float FOV = glm::radians(30.0f);
constexpr float NEARPLANE = 1.0f;
constexpr float FARPLANE = 100.0f;

constexpr float CAMERA_VELOCITY = 1.5f;

class CameraFree
{
public:
	CameraFree(glm::vec3 position)
		:
		position(position), up(GLOBAL_UP),
		nearPlane(NEARPLANE), farPlane(FARPLANE), fov(FOV),
		moveForward(false), moveBackward(false), moveLeft(false), moveRight(false),
		hasBeenMoved(true), hasBeenRotated(true),
		theta(0.0), phi(0.0)
	{
		updateBaseVectors();
		updateViewMatrix();
		updateProjectionMatrix();
	}

	glm::mat4 getViewMatrix() const {
		return viewMatrix;
	}

	glm::mat4 getProjectionMatrix() const {
		return projectionMatrix;
	}

	void updateViewMatrix()
	{
		bool needsUpdate = false;
		if (moveForward || moveBackward || moveLeft || moveRight)
		{
			moveCamera();
			updateBaseVectors();
			needsUpdate = true;
		}
		if (hasBeenRotated)
		{
			updateBaseVectors();
			needsUpdate = true;
		}

		if (needsUpdate) {
			viewMatrix = glm::mat4(
				right.x, up.x, forward.x, 0,
				right.y, up.y, forward.y, 0,
				right.z, up.z, forward.z, 0,
				-Linalg::dot(right, position), -Linalg::dot(up, position), -Linalg::dot(forward, position), 1);
		}
	}

	void updateBaseVectors() {
		float phi_rad = static_cast<float>(glm::radians(phi));
		float theta_rad = static_cast<float>(glm::radians(theta));

		auto direction = glm::vec3(0.0);
		direction.x = cosf(phi_rad) * cosf(theta_rad);
		direction.y = sinf(phi_rad);
		direction.z = cosf(phi_rad) * sinf(theta_rad);

		forward = Linalg::normalize(direction);
		right = Linalg::normalize(Linalg::cross(forward, GLOBAL_UP));
		up = Linalg::normalize(Linalg::cross(right, forward));
	}

	void updateProjectionMatrix() {
		int width, height;
		glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
		float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

		float top = glm::abs(nearPlane) * glm::tan(fov / 2.0f);
		float bottom = -top;
		float right = top * aspect_ratio;
		float left = -right;

		projectionMatrix = glm::mat4(
			(2.0f * nearPlane) / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
			0.0f, (2.0f * nearPlane) / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
			0.0f, 0.0f, -(farPlane + nearPlane) / (farPlane - nearPlane), -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane),
			0.0f, 0.0f, -1.0f, 0.0f);
	}

	void moveCamera() {
		glm::vec3 moveDirection = glm::vec3(0.0f);
		if (moveForward) moveDirection -= forward;
		if (moveBackward) moveDirection += forward;
		if (moveLeft) moveDirection -= right;
		if (moveRight) moveDirection += right;

		// Evita que o jogador voe pelo mapa.
		moveDirection.y = 0.0f;

		position += moveDirection * CAMERA_VELOCITY * *dt;

		position.y = glm::perlin(glm::vec2(position.x, position.z) * PERLINSCALINGFACTOR) + 1.0f;
	}

	void bindDeltaTime(std::shared_ptr<float> dt) {
		this->dt = dt;
	}

	bool moveForward, moveBackward, moveLeft, moveRight;
	bool hasBeenMoved, hasBeenRotated;
	glm::vec3 position;

	// View matrix
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	double theta;
	double phi;

	// Projection matrix
	float nearPlane;
	float farPlane;
	float fov;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	std::shared_ptr<float> dt;
};
#endif