#include <entity.hpp>
#include <collision.hpp>

void Enemy::translate(glm::vec3 t) {
	position += t;

	for (auto& plane : hitbox.planes) {
		plane.bottomLeft += t;
		plane.bottomRight += t;
		plane.topLeft += t;
		plane.topRight += t;
		plane.initVAO();
	}
}

void Enemy::rotate(glm::vec3 angles)
{
	rotation.x += glm::radians(angles.x);
	rotation.y += glm::radians(angles.y);
	rotation.z += glm::radians(angles.z);

	for (auto& plane : hitbox.planes) {
		plane.rotate(angles, position);
		plane.initVAO();
	}

}

void Enemy::scale(float s) {
	scaling = s;
	for (auto& plane : hitbox.planes)
		plane.scale(s, position);
}

void Enemy::draw(Shader& shader, CameraFree& camera)
{
	model.render(shader, camera, position, rotation, scaling);

	// Descomentar se precisar ver a hitbox
	//std::for_each(hitbox.planes.begin(), hitbox.planes.end(), [&](Plane& plane) {
	//	plane.render(shader, camera);
	//	});
}
