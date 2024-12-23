#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <shader.hpp>
#include <camera.hpp>
#include <GLM/vec3.hpp>
#include <vector>
#include <string>
#include <numeric>

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
	float range;

	Ray(CameraFree& camera, float r) {
		origin = camera.position;
		direction = -camera.forward;
		range = r;
	}
};

struct Plane {
public:
	glm::vec3 bottomLeft;
	glm::vec3 bottomRight;
	glm::vec3 topLeft;
	glm::vec3 topRight;
	glm::vec3 normal;

	Plane(glm::vec3 bl, glm::vec3 br, glm::vec3 tr, glm::vec3 tl) {
		bottomLeft = bl;
		bottomRight = br;
		topRight = tr;
		topLeft = tl;
		normal = glm::normalize(glm::cross(br - bl, tl - bl));

		initVAO();
	}

	void scale(float s, glm::vec3 center) {
		glm::mat4 t = Linalg::translate(-center);
		glm::mat4 t_inv = Linalg::translate(center);

		bottomLeft = glm::vec3(t * glm::vec4(bottomLeft, 0.0f));
		bottomRight = glm::vec3(t * glm::vec4(bottomRight, 0.0f));
		topLeft = glm::vec3(t * glm::vec4(topLeft, 0.0f));
		topRight = glm::vec3(t * glm::vec4(topRight, 0.0f));

		bottomLeft *= s;
		bottomRight *= s;
		topLeft *= s;
		topRight *= s;

		bottomLeft = glm::vec3(t_inv * glm::vec4(bottomLeft, 0.0f));
		bottomRight = glm::vec3(t_inv * glm::vec4(bottomRight, 0.0f));
		topLeft = glm::vec3(t_inv * glm::vec4(topLeft, 0.0f));
		topRight = glm::vec3(t_inv * glm::vec4(topRight, 0.0f));

		glDeleteVertexArrays(1, &VAO);
		initVAO();
	}

	void translate(glm::vec3& translation) {
		bottomLeft += translation;
		bottomRight += translation;
		topLeft += translation;
		topRight += translation;

		glDeleteVertexArrays(1, &VAO);
		initVAO();
	}

	void rotate(glm::vec3 angles, glm::vec3 center) {
		glm::mat4 t = Linalg::translate(-center);
		glm::mat4 t_inv = Linalg::translate(center);

		bottomLeft = glm::vec3(t * glm::vec4(bottomLeft, 0.0f));
		bottomRight = glm::vec3(t * glm::vec4(bottomRight, 0.0f));
		topLeft = glm::vec3(t * glm::vec4(topLeft, 0.0f));
		topRight = glm::vec3(t * glm::vec4(topRight, 0.0f));

		glm::mat4 rotationMatrixX = glm::mat4(1.0f);
		glm::mat4 rotationMatrixY = glm::mat4(1.0f);
		glm::mat4 rotationMatrixZ = glm::mat4(1.0f);

		if (angles.x != 0)
			rotationMatrixX = Linalg::rotateX(glm::radians(angles.x));
		if (angles.y != 0)
			rotationMatrixY = Linalg::rotateY(glm::radians(angles.y));
		if (angles.z != 0)
			rotationMatrixZ = Linalg::rotateZ(glm::radians(angles.z));

		glm::mat4 rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

		bottomLeft = glm::vec3(rotationMatrix * glm::vec4(bottomLeft, 0.0f));
		bottomRight = glm::vec3(rotationMatrix * glm::vec4(bottomRight, 0.0f));
		topLeft = glm::vec3(rotationMatrix * glm::vec4(topLeft, 0.0f));
		topRight = glm::vec3(rotationMatrix * glm::vec4(topRight, 0.0f));

		bottomLeft = glm::vec3(t_inv * glm::vec4(bottomLeft, 0.0f));
		bottomRight = glm::vec3(t_inv * glm::vec4(bottomRight, 0.0f));
		topLeft = glm::vec3(t_inv * glm::vec4(topLeft, 0.0f));
		topRight = glm::vec3(t_inv * glm::vec4(topRight, 0.0f));

		normal = Linalg::normalize(Linalg::cross(bottomRight - bottomLeft, topLeft - bottomLeft));

		glDeleteVertexArrays(1, &VAO);
		initVAO();
	}

	bool testCollision(Ray ray) const
	{
		float denominator = Linalg::dot(ray.direction, normal);
		if (std::abs(denominator) < 1e-6) {
			return false;
		}

		float numerator = Linalg::dot(bottomLeft - ray.origin, normal);

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

	// PARA DEBUG APENAS
	GLuint VAO;

	void initVAO() {
		glm::vec3 vertices[] = {
			bottomLeft,
			bottomRight,
			topRight,
			topLeft
		};

		std::vector<unsigned int> indices = {
			0, 1, 2, 0, 2, 3,
		};

		GLuint VBO = 0, indiceBuffer = 0;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &indiceBuffer);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void render(Shader& shader, CameraFree& camera) const {
		glBindVertexArray(VAO);
		shader.use();

		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = camera.getProjectionMatrix();
		glm::mat4 id = glm::mat4(1.0f);

		shader.setVec3("translation", glm::vec3(0.0f));
		shader.setVec3("rotation", glm::vec3(0.0f));
		shader.setFloat("scaling", 1.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		shader.reset();
		glBindVertexArray(0);
	}
};


class Hitbox {
public:
	Hitbox(std::vector<glm::vec3> points) {
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

	bool testRayCollision(Ray ray) const {
		return std::any_of(planes.begin(), planes.end(), [&](const Plane& plane) {
			return plane.testCollision(ray);
			});
	}

	std::vector<Plane> planes;

private:
	GLuint VAO;
};

#endif