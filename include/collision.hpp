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

class Plane {
public:
	glm::vec3 bottomLeft;
	glm::vec3 bottomRight;
	glm::vec3 topLeft;
	glm::vec3 topRight;
	glm::vec3 normal;

	Plane() = default;

	Plane(glm::vec3 bl, glm::vec3 br, glm::vec3 tr, glm::vec3 tl) {
		bottomLeft = bl;
		bottomRight = br;
		topRight = tr;
		topLeft = tl;
		normal = glm::normalize(glm::cross(br - bl, tl - bl));
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
	}

	void translate(glm::vec3& translation) {
		bottomLeft += translation;
		bottomRight += translation;
		topLeft += translation;
		topRight += translation;
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
	}

	bool intersectRayTest(const Ray& ray) const
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
};

class Collider {
public:
	virtual bool intersect(const Collider& other) const = 0;
	virtual bool intersectRay(const Ray& ray) const = 0;
	virtual void translate(glm::vec3 translation) = 0;
	virtual void rotate(glm::vec3 rotation, glm::vec3 center) = 0;
	virtual void scale(float scaling, glm::vec3 center) = 0;
	virtual void render(Shader& shader, CameraFree& camera, glm::vec3 position, glm::vec3 rotation, float scaling) const = 0;

	virtual std::unique_ptr<Collider> clone() const = 0;

	virtual ~Collider() = default;
};

class Hitbox : public Collider {
public:
	std::vector<Plane> planes;
	GLuint VAO;
	unsigned int triangleCount;

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

		std::vector<unsigned int> indices = {
			0, 1, 2,
			2, 3, 0,

			// Back face
			4, 5, 6,
			6, 7, 4,

			// Left face
			0, 4, 7,
			7, 3, 0,

			// Right face
			1, 5, 6,
			6, 2, 1,

			// Top face
			3, 2, 6,
			6, 7, 3,

			// Bottom face
			0, 1, 5,
			5, 4, 0
		};
		triangleCount = indices.size();

		GLuint VBO = 0, indices_buffer = 0;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &indices_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(Position);

		glBindVertexArray(0);
	}

	Hitbox(const Hitbox& other) noexcept {
		this->planes = other.planes;
		this->VAO = other.VAO;
		this->triangleCount = other.triangleCount;
	}

	Hitbox& operator=(const Hitbox& other) {
		if (this != &other) {
			this->planes = other.planes;
		}
		return *this;
	}

	std::unique_ptr<Collider> clone() const override {
		return std::make_unique<Hitbox>(*this);
	}

	bool intersect(const Collider& collider) const override {
		return false;
	}

	bool intersectRay(const Ray& ray) const override {
		return std::any_of(planes.begin(), planes.end(), [&](const Plane& plane) {
			return plane.intersectRayTest(ray);
			});
	}

	void translate(glm::vec3 translation) override {
		for (auto& plane : planes) {
			plane.bottomLeft += translation;
			plane.bottomRight += translation;
			plane.topLeft += translation;
			plane.topRight += translation;
		}
	}

	void rotate(glm::vec3 rotation, glm::vec3 center) override {
		glm::mat4 translateToOriginMatrix = Linalg::translate(-center);
		glm::mat4 TranslateToOriginInverseMatrix = Linalg::translate(center);

		for (auto& plane : planes) {
			plane.bottomLeft = glm::vec3(translateToOriginMatrix * glm::vec4(plane.bottomLeft, 0.0f));
			plane.bottomRight = glm::vec3(translateToOriginMatrix * glm::vec4(plane.bottomRight, 0.0f));
			plane.topLeft = glm::vec3(translateToOriginMatrix * glm::vec4(plane.topLeft, 0.0f));
			plane.topRight = glm::vec3(translateToOriginMatrix * glm::vec4(plane.topRight, 0.0f));

			glm::mat4 rotationMatrixX = glm::mat4(1.0f);
			glm::mat4 rotationMatrixY = glm::mat4(1.0f);
			glm::mat4 rotationMatrixZ = glm::mat4(1.0f);

			if (rotation.x != 0)
				rotationMatrixX = Linalg::rotateX(glm::radians(rotation.x));
			if (rotation.y != 0)
				rotationMatrixY = Linalg::rotateY(glm::radians(rotation.y));
			if (rotation.z != 0)
				rotationMatrixZ = Linalg::rotateZ(glm::radians(rotation.z));

			glm::mat4 rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

			plane.bottomLeft = glm::vec3(rotationMatrix * glm::vec4(plane.bottomLeft, 0.0f));
			plane.bottomRight = glm::vec3(rotationMatrix * glm::vec4(plane.bottomRight, 0.0f));
			plane.topLeft = glm::vec3(rotationMatrix * glm::vec4(plane.topLeft, 0.0f));
			plane.topRight = glm::vec3(rotationMatrix * glm::vec4(plane.topRight, 0.0f));

			plane.bottomLeft = glm::vec3(TranslateToOriginInverseMatrix * glm::vec4(plane.bottomLeft, 0.0f));
			plane.bottomRight = glm::vec3(TranslateToOriginInverseMatrix * glm::vec4(plane.bottomRight, 0.0f));
			plane.topLeft = glm::vec3(TranslateToOriginInverseMatrix * glm::vec4(plane.topLeft, 0.0f));
			plane.topRight = glm::vec3(TranslateToOriginInverseMatrix * glm::vec4(plane.topRight, 0.0f));

			plane.normal = Linalg::normalize(Linalg::cross(plane.bottomRight - plane.bottomLeft, plane.topLeft - plane.bottomLeft));
		}
	}

	void scale(float scaling, glm::vec3 center) override {
		for (auto& plane : planes) {
			glm::mat4 t = Linalg::translate(-center);
			glm::mat4 t_inv = Linalg::translate(center);

			plane.bottomLeft = glm::vec3(t * glm::vec4(plane.bottomLeft, 0.0f));
			plane.bottomRight = glm::vec3(t * glm::vec4(plane.bottomRight, 0.0f));
			plane.topLeft = glm::vec3(t * glm::vec4(plane.topLeft, 0.0f));
			plane.topRight = glm::vec3(t * glm::vec4(plane.topRight, 0.0f));

			plane.bottomLeft *= scaling;
			plane.bottomRight *= scaling;
			plane.topLeft *= scaling;
			plane.topRight *= scaling;

			plane.bottomLeft = glm::vec3(t_inv * glm::vec4(plane.bottomLeft, 0.0f));
			plane.bottomRight = glm::vec3(t_inv * glm::vec4(plane.bottomRight, 0.0f));
			plane.topLeft = glm::vec3(t_inv * glm::vec4(plane.topLeft, 0.0f));
			plane.topRight = glm::vec3(t_inv * glm::vec4(plane.topRight, 0.0f));
		}
	}

	void render(Shader& shader, CameraFree& camera, glm::vec3 position, glm::vec3 rotation, float scaling) const override {
		glm::mat4 v = camera.getViewMatrix();
		glm::mat4 p = camera.getProjectionMatrix();

		shader.use();
		shader.setMat4("view", v);
		shader.setMat4("projection", p);

		shader.setVec3("translation", position);
		shader.setVec3("rotation", rotation);
		shader.setFloat("scaling", scaling);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, triangleCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		shader.unbind();
	}
};

class Hitsphere : public Collider {
public:
	float radius;
	glm::vec3 center;

	Hitsphere(float radius, glm::vec3 position) {
		this->radius = radius;
		this->center = position;
	}

	Hitsphere(const Hitsphere& other) noexcept {
		this->radius = other.radius;
		this->center = other.center;
	}

	Hitsphere& operator=(const Hitsphere& other) {
		if (this != &other) {
			this->radius = other.radius;
			this->center = other.center;
		}
		return *this;
	}

	bool intersect(const Collider& other)const  override { return false; }

	// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection#:~:text=Calculation%20using%20vectors%20in%203D
	bool intersectRay(const Ray& ray) const override {
		// Se o vetor da direção do raio for normalizado temos a = 1
		float a = 1.0f;
		float b = 2 * Linalg::dot(ray.direction, ray.origin - center);
		float c = Linalg::dot(ray.origin - center, ray.origin - center) - radius * radius;

		float discriminant = b * b - 4 * a * c;

		if (discriminant < std::numeric_limits<float>::epsilon()) return false;

		// https://math.stackexchange.com/questions/866331/numerically-stable-algorithm-for-solving-the-quadratic-equation-when-a-is-very
		float x1 = 2 * c / (-b + sqrtf(discriminant));
		float x2 = 2 * c / (-b - sqrtf(discriminant));

		glm::vec3 intersectionPoint1 = ray.origin + x1 * ray.direction;
		glm::vec3 intersectionPoint2 = ray.origin + x2 * ray.direction;

		float dist1 = Linalg::length(intersectionPoint1 - ray.origin);
		float dist2 = Linalg::length(intersectionPoint2 - ray.origin);

		if (std::min(dist1, dist2) <= ray.range) return true;

		return false;
	}

	std::unique_ptr<Collider> clone() const {
		return std::make_unique<Hitsphere>(*this);
	}

	void translate(glm::vec3 translation) override {
		center += translation;
	}

	void rotate(glm::vec3 rotation, glm::vec3 center) override {}

	void scale(float scaling, glm::vec3 center) override {
		radius *= scaling;
	}

	void render(Shader& shader, CameraFree& camera, glm::vec3 position, glm::vec3 rotation, float scaling) const override {
		glm::mat4 v = camera.getViewMatrix();
		glm::mat4 p = camera.getProjectionMatrix();

		shader.use();
		shader.setMat4("view", v);
		shader.setMat4("projection", p);

		shader.setVec3("translation", position);
		shader.setVec3("rotation", rotation);
		shader.setFloat("scaling", scaling);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, numTriangles, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		shader.unbind();
	}

	GLuint VAO;
	unsigned int numTriangles;
};

#endif