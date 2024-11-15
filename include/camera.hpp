#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <memory>
#include <GLM/vec4.hpp>
#include <GLM/mat4x4.hpp>

class CameraFree
{
public:
    CameraFree(glm::vec3 position, glm::vec3 target, glm::vec3 up);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix() const; 
    
    void updateVectors();
    void updateView();
    void updateProjection();
    void moveCamera();

    void linkDeltaTime(std::shared_ptr<float> dt) {
        this->dt = dt;
    }

    bool moveForward, moveBackward, moveLeft, moveRight, moveUp, moveDown;
    bool hasBeenMoved, hasBeenRotated;
    float theta;
    float phi;

private:

    glm::vec3 position;
    glm::vec3 target;

    // View matrix
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    // Projection matrix
    float nearPlane;
    float farPlane;
    float fov;

    // Matrices
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    std::shared_ptr<float> dt;
};

#endif