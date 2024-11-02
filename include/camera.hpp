#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>

class CameraFree
{
public:
    CameraFree(glm::vec3 eye, glm::vec3 view, glm::vec3 up, float nearPlane, float farPlane, float fov);

    void moveCamera(glm::vec3 direction, float dt);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

private:
    // View matrix
    glm::vec3 eye;
    glm::vec3 view;
    glm::vec3 up;

    // Projection matrix
    float nearPlane;
    float farPlane;
    float fov;
};

#endif