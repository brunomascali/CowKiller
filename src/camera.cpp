// Bibliotecas externas
#include <GLM/glm.hpp>
#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>

// Bibliotecas próprias
#include <camera.hpp>

CameraFree::CameraFree(glm::vec3 eye, glm::vec3 view, glm::vec3 up, float nearPlane, float farPlane, float fov)
    : eye(eye), view(view), up(up), nearPlane(nearPlane), farPlane(farPlane), fov(fov) {}

void CameraFree::moveCamera(glm::vec3 direction, float dt)
{
    eye += dt * direction;
}

glm::mat4 CameraFree::getViewMatrix()
{
    glm::vec3 w = -glm::normalize(view);
    glm::vec3 u = glm::normalize(glm::cross(up, w));
    glm::vec3 v = glm::cross(w, u);

    return glm::mat4(
        u.x, v.x, w.x, 0,
        u.y, v.y, w.y, 0,
        u.z, v.z, w.z, 0,
        -glm::dot(u, eye), -glm::dot(v, eye), -glm::dot(w, eye), 1);
}

glm::mat4 CameraFree::getProjectionMatrix()
{
    float aspect_ratio = 1.0f;

    float top = glm::abs(nearPlane) * glm::tan(fov / 2.0f);
    float bottom = -top;
    float right = top * aspect_ratio;
    float left = -right;

    auto M = glm::mat4(
        2.0f / (right - left), 0, 0, 0,
        0, 2.0f / (top - bottom), 0, 0,
        0, 0, 2.0f / (farPlane - nearPlane), 0,
        -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farPlane + nearPlane) / (farPlane - nearPlane), 1.0);

    auto P = glm::mat4(
        nearPlane, 0, 0, 0,
        0, nearPlane, 0, 0,
        0, 0, nearPlane + farPlane, 1,
        0, 0, -nearPlane * farPlane, 0);

    // return M * P;

    return glm::mat4(
        (2.0f * nearPlane) / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
        0.0f, (2.0f * nearPlane) / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
        0.0f, 0.0f, -(farPlane + nearPlane) / (farPlane - nearPlane), -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane),
        0.0f, 0.0f, -1.0f, 0.0f);
}
