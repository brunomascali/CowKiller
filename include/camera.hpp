#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <memory>
#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>

constexpr glm::vec3 GLOBAL_UP = glm::vec3(0.0f, 1.0f, 0.0f);

class CameraFree
{
public:
    CameraFree(glm::vec3 position);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix() const; 
    
    void updateBaseVectors();
    void updateViewMatrix();
    void updateProjectionMatrix();
    void moveCamera();

    void bindDeltaTime(std::shared_ptr<float> dt) {
        this->dt = dt;
    }

    bool moveForward, moveBackward, moveLeft, moveRight;
    bool hasBeenMoved, hasBeenRotated;
    double theta;
    double phi;
    glm::vec3 position;
    glm::vec3 forward;

    // View matrix
    glm::vec3 right;
    glm::vec3 up;

    // Projection matrix
    float nearPlane;
    float farPlane;
    float fov;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    // Delta time
    std::shared_ptr<float> dt;
};

#endif