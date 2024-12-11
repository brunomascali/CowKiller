#include "linalg.hpp"

glm::mat4 Linalg::rotateX(float angle)
{
	float cosTheta = cos(angle);
	float sinTheta = sin(angle);

	return glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, cosTheta, -sinTheta, 0.0,
		0.0, sinTheta, cosTheta, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

glm::mat4 Linalg::rotateY(float angle) {
	float cosTheta = cos(angle);
	float sinTheta = sin(angle);

	return glm::mat4(
		cosTheta, 0.0, sinTheta, 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sinTheta, 0.0, cosTheta, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

glm::mat4 Linalg::rotateZ(float angle) {
	float cosTheta = cos(angle);
	float sinTheta = sin(angle);

	return glm::mat4(
		cosTheta, -sinTheta, 0.0, 0.0,
		sinTheta, cosTheta, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

glm::mat4 Linalg::scaleMatrix(float s)
{
	return glm::mat4(
		s, 0.0f, 0.0f, 0.0f,
		0.0f, s, 0.0f, 0.0f,
		0.0f, 0.0f, s, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

glm::mat4 Linalg::translate(glm::vec3 translation)
{
	return glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		translation.x, translation.y, translation.z, 1.0
	);
}

float Linalg::dot(glm::vec3 v1, glm::vec3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Linalg::length(glm::vec3 v)
{
	return sqrtf(Linalg::dot(v, v));
}

glm::vec3 Linalg::normalize(glm::vec3 v)
{
	return v / Linalg::length(v);
}

glm::vec3 Linalg::cross(glm::vec3 v1, glm::vec3 v2)
{
	return glm::vec3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

glm::vec3 Linalg::abs(glm::vec3 v)
{
	float x = v.x >= 0 ? v.x : -v.x;
	float y = v.y >= 0 ? v.y : -v.y;
	float z = v.z >= 0 ? v.z : -v.z;
	return glm::vec3(x, y, z);
}

