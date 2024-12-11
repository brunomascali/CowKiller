#ifndef LINALG_HPP
#define LINALG_HPP

#include <GLM/ext/matrix_float4x4.hpp>

namespace Linalg {
	glm::mat4 rotateX(float angle);
	glm::mat4 rotateY(float angle);
	glm::mat4 rotateZ(float angle);
	glm::mat4 scaleMatrix(float s);
	glm::mat4 translate(glm::vec3 translation);
	float dot(glm::vec3 v1, glm::vec3 v2);
	float length(glm::vec3 v);
	glm::vec3 normalize(glm::vec3 v);
	glm::vec3 cross(glm::vec3 v1, glm::vec3 v2);
	glm::vec3 abs(glm::vec3 v);
}

#endif
