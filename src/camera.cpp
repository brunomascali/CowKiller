// Bibliotecas externas
#include <GLM/glm.hpp>
#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>
#include <GLFW/glfw3.h>

// Bibliotecas próprias
#include <camera.hpp>
#include <iostream>
#include <GLM/gtx/string_cast.hpp>
#include <GLM/gtx/string_cast.hpp>

CameraFree::CameraFree(glm::vec3 position, glm::vec3 target, glm::vec3 up)
    : 
    position(position), target(target), up(up), 
    nearPlane(1.0f), farPlane(100.0f), fov(glm::radians(30.0f)),
    moveForward(false), moveBackward(false), moveLeft(false), moveRight(false), 
    hasBeenMoved(false), hasBeenRotated(false),
    theta(0.0f), phi(0.0f)
{
    this->updateCameraBasisVectors();
    this->updateViewMatrix();
    this->updateProjectionMatrix();
}

glm::mat4 CameraFree::getViewMatrix()
{
    if (moveForward || moveBackward || moveLeft || moveRight)
    {
        this->moveCamera();
        this->updateCameraBasisVectors();
        this->updateViewMatrix();
    }
    if (hasBeenRotated)
    {
        this->updateCameraBasisVectors();
        this->updateViewMatrix();
    }
    return this->viewMatrix;
}

glm::mat4 CameraFree::getProjectionMatrix() const
{
    return this->projectionMatrix;
}

void CameraFree::updateCameraBasisVectors()
{
    float phi_rad = glm::radians(phi);
    float theta_rad = glm::radians(theta);

    auto direction = glm::vec3(0.0f);
    direction.x = cosf(phi_rad) * cosf(theta_rad);
    direction.y = sinf(phi_rad);
    direction.z = cosf(phi_rad) * sinf(theta_rad);

    this->forward = glm::normalize(direction);
    this->right = glm::normalize(glm::cross(this->forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    this->up = glm::normalize(glm::cross(this->right, this->forward));
}

void CameraFree::updateViewMatrix()
{
    this->viewMatrix = glm::mat4(
        right.x, up.x, forward.x, 0,
        right.y, up.y, forward.y, 0,
        right.z, up.z, forward.z, 0,
        -glm::dot(right, position), -glm::dot(up, position), -glm::dot(forward, position), 1);
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
    float velocity = 20.0f;
    float dt = *this->dt;

    if (moveForward) {
        position -= glm::vec3(1.0f, 0.0f, 1.0f) * forward * dt * 20.0f;
    }
    if (moveBackward)
    {
        position += glm::vec3(1.0f, 0.0f, 1.0f) * forward * dt * 20.0f;
    }
    if (moveLeft)
    {
        position -= glm::vec3(1.0f, 0.0f, 1.0f) * right * dt * 20.0f;
    }
    if (moveRight)
    {
        position += glm::vec3(1.0f, 0.0f, 1.0f) * right * dt * 20.0f;
    }
}