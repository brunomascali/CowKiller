#include <skybox.hpp>
#include <model.hpp>

Skybox::Skybox()
{
	std::vector<float> vertices = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	GLuint VBO = 0;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(Location::Position);
	glVertexAttribPointer(Location::Position, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<const void*>(0));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	texture = Texture({ "right.png",
			"left.png",
			"top.png",
			"bottom.png",
			"front.png",
			"back.png" });
}

void Skybox::render(Shader& shader, CameraFree& camera) const
{
	glm::mat4 v = glm::mat4(glm::mat3(camera.getViewMatrix()));
	glm::mat4 p = camera.getProjectionMatrix();

	glDepthFunc(GL_LEQUAL);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

	shader.use();
	shader.setMat4("view", v);
	shader.setMat4("projection", p);
	shader.setInt("skybox", 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);
	shader.reset();

	glDepthFunc(GL_LESS);
}