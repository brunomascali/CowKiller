#include <vector>
#include <collision.hpp>
#include <camera.hpp>
#include <shader.hpp>
#include <mesh.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>
#include <iostream>
#include <glad/glad.h>
#include <string>
#include <linalg.hpp>

//     a7------a6
//    /|       /|
//   / |      / |
//  a3-------a2 |
//  |  |     |  |
//  |  a4----|-a5
//  | /      | /
//  |/       |/
//  a0-------a1

Hitbox::Hitbox(std::vector<glm::vec3> points)
{
	Plane a = Plane(points[0], points[1], points[2], points[3]);
	Plane b = Plane(points[4], points[5], points[6], points[7]);
	Plane c = Plane(points[4], points[0], points[3], points[7]);
	Plane d = Plane(points[1], points[5], points[6], points[2]);
	Plane e = Plane(points[3], points[2], points[6], points[7]);
	Plane f = Plane(points[0], points[1], points[5], points[4]);

	planes.push_back(a);
	planes.push_back(b);
	planes.push_back(c);
	planes.push_back(d);
	planes.push_back(e);
	planes.push_back(f);

}

void Plane::scale(float s, glm::vec3 center)
{
	bottomLeft = center + (bottomLeft - center) * s;
	bottomRight = center + (bottomRight - center) * s;
	topLeft = center + (topLeft - center) * s;
	topRight = center + (topRight - center) * s;

	glDeleteVertexArrays(1, &VAO);

	initVAO();
}

void Plane::translate(glm::vec3& translation)
{
	bottomLeft += translation;
	bottomRight += translation;
	topLeft += translation;
	topRight += translation;
}

void Plane::rotate(glm::vec3 angles, glm::vec3 center)
{
	bottomLeft -= center;
	bottomLeft -= center;
	topLeft -= center;
	topRight -= center;

	glm::mat4 rotationMatrixX = glm::mat4(1.0f);
	glm::mat4 rotationMatrixY = glm::mat4(1.0f);
	glm::mat4 rotationMatrixZ = glm::mat4(1.0f);

	if (angles.x != 0)
		rotationMatrixX = Linalg::rotateX(glm::radians(angles.x));
	if (angles.y != 0)
		rotationMatrixY = Linalg::rotateY(glm::radians(angles.y));
	if (angles.z != 0)
		rotationMatrixZ = Linalg::rotateZ(glm::radians(angles.z));

	// Precisa investigar pq essa matriz bagunça as normais do plano
	//glm::mat4 rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));

	auto rotatePoint = [&rotationMatrix, &center](glm::vec3& point) {
		point = glm::vec3(rotationMatrix * glm::vec4(point - center, 1.0f)) + center;
		};

	rotatePoint(bottomLeft);
	rotatePoint(bottomRight);
	rotatePoint(topLeft);
	rotatePoint(topRight);

	planeNormalVector = Linalg::normalize(Linalg::cross(bottomRight - bottomLeft, topLeft - bottomLeft));
}

bool Plane::testCollision(Ray ray) const
{
	float denominator = Linalg::dot(ray.direction, planeNormalVector);
	if (std::abs(denominator) < 1e-6) {
		return false;
	}

	float numerator = Linalg::dot(bottomLeft - ray.origin, planeNormalVector);

	float t = numerator / denominator;
	if (t < 0.0f || t > ray.range) {
		return false;
	}

	glm::vec3 intersectionPoint = ray.origin + t * ray.direction;

	glm::vec3 e1 = bottomRight - bottomLeft;
	glm::vec3 e2 = topLeft - bottomLeft;
	glm::vec3 intPointRelativeToPlane = intersectionPoint - bottomLeft;

	float proje1 = Linalg::dot(intPointRelativeToPlane, e1) / Linalg::dot(e1, e1);
	float proje2 = Linalg::dot(intPointRelativeToPlane, e2) / Linalg::dot(e2, e2);

	if (proje1 >= 0 && proje1 <= 1.0f && proje2 >= 0 && proje2 <= 1.0f)
		return true;

	return false;
}

bool Hitbox::testRayCollision(Ray ray) const
{
	return std::any_of(planes.begin(), planes.end(), [&](const Plane& plane) {
		return plane.testCollision(ray);
		});
}