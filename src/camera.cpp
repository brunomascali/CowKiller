// Bibliotecas externas
#include <iostream>
#include <terrain.hpp>
#include <GLM/glm.hpp>
#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>
#include <GLFW/glfw3.h>

// Bibliotecas próprias
#include <camera.hpp>
#include <GLM/gtc/noise.hpp>
#include <linalg.hpp>

constexpr float MOVE_VELOCITY = 2.0f;

CameraFree::CameraFree(glm::vec3 position)
    : 
    position(position), up(glm::vec3(0.0f, 1.0f, 0.0f)), 
    nearPlane(1.0f), farPlane(100.0f), fov(glm::radians(30.0f)),
    moveForward(false), moveBackward(false), moveLeft(false), moveRight(false), 
    hasBeenMoved(true), hasBeenRotated(true),
    theta(0.0), phi(0.0)
{
    this->updateBaseVectors();
    this->updateViewMatrix();
    this->updateProjectionMatrix();
}

glm::mat4 CameraFree::getViewMatrix()
{
    if (moveForward || moveBackward || moveLeft || moveRight)
    {
        this->moveCamera();
        this->updateBaseVectors();
        this->updateViewMatrix();
    }
    if (hasBeenRotated)
    {
        this->updateBaseVectors();
        this->updateViewMatrix();
    }
    return this->viewMatrix;
}

glm::mat4 CameraFree::getProjectionMatrix() const
{
    return this->projectionMatrix;
}

void CameraFree::updateBaseVectors()
{
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

void CameraFree::updateViewMatrix()
{
    viewMatrix = glm::mat4(
        right.x, up.x, forward.x, 0,
        right.y, up.y, forward.y, 0,
        right.z, up.z, forward.z, 0,
        -Linalg::dot(right, position), -Linalg::dot(up, position), -Linalg::dot(forward, position), 1);
}

void CameraFree::updateProjectionMatrix()
{
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

    float top = glm::abs(nearPlane) * glm::tan(fov / 2.0f);
    float bottom = -top;
    float right = top * aspect_ratio;
    float left = -right;

    this->projectionMatrix = glm::mat4(
        (2.0f * nearPlane) / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
        0.0f, (2.0f * nearPlane) / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
        0.0f, 0.0f, -(farPlane + nearPlane) / (farPlane - nearPlane), -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane),
        0.0f, 0.0f, -1.0f, 0.0f);
}

void CameraFree::moveCamera()
{
    glm::vec3 moveDirection = glm::vec3(0.0f);
    if (moveForward) moveDirection -= forward;
    if (moveBackward) moveDirection += forward;
    if (moveLeft) moveDirection -= right;
    if (moveRight) moveDirection += right;

    // Evita que o jogador voe pelo mapa.
    moveDirection.y = 0.0f;
    
    position += moveDirection * MOVE_VELOCITY * *dt;

    position.y = glm::perlin(glm::vec2(position.x, position.z) * perlinScalingFactor) + 2.0f;
}
