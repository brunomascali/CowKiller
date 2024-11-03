#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>

class CameraFree
{
public:
    CameraFree(glm::vec3 eye, glm::vec3 view, glm::vec3 up, float nearPlane, float farPlane, float fov);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    
    void updateCameraPosition(float dt);

    bool moveForward, moveBackward, moveLeft, moveRight;
private:
    void moveCamera(glm::vec3 direction, float dt);

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