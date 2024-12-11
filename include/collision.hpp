#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <shader.hpp>
#include <camera.hpp>
#include <GLM/vec3.hpp>
#include <vector>
#include <string>

struct Plane;
struct Ray;

struct Plane {
	glm::vec3 bottomLeft;
	glm::vec3 bottomRight;
	glm::vec3 topLeft;
	glm::vec3 topRight;
	glm::vec3 planeNormalVector;

	Plane(glm::vec3 bl, glm::vec3 br, glm::vec3 tr, glm::vec3 tl) {
		bottomLeft = bl;
		bottomRight = br;
		topRight = tr;
		topLeft = tl;

		planeNormalVector = glm::normalize(glm::cross(br - bl, tl - bl));
		initVAO();
	}

	void scale(float s, glm::vec3 center);
	void translate(glm::vec3& translation);
	void rotate(glm::vec3 angles, glm::vec3 center);

	bool testCollision(Ray ray) const;

	// PARA DEBUG APENAS
	GLuint VAO;

	void initVAO() {
		glm::vec3 vertices[] = {
			bottomLeft,
			bottomRight,
			topLeft,
			topLeft,
			bottomRight,
			topRight
		};

		GLuint VBO = 0;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void render(Shader& shader, CameraFree& camera) {
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

		glDrawArrays(GL_TRIANGLES, 0, 6);

		shader.reset();
		glBindVertexArray(0);
	}
};

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

//     .-------p2
//    /|       /|
//   / |      / |
//  /--------/  |
//  |  |     |  |
//  |  ------|--|
//  | /      | /
//  |/       |/
//  p1-------/

class Hitbox {
public:
	Hitbox(std::vector<glm::vec3> points);

	std::vector<Plane> planes;

	bool testRayCollision(Ray ray) const;
private:
	GLuint VAO;
};

#endif