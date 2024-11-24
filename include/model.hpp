#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <filesystem>

#include <GLM/vec3.hpp>
#include <GLM/mat4x4.hpp>

#include <shader.hpp>
#include <texture.hpp>
#include <camera.hpp>
#include <mesh.hpp>

class Model {
public: 
    Model(std::filesystem::path modelPath);
    void createMesh(const aiScene* scene, const aiNode* node);

    void render(const Shader& shader, CameraFree& camera);
    
    void translate(glm::vec3 t) { position += t; };
    void rotateX(float angle) { rotation.x += angle; };
    void rotateY(float angle) { rotation.y += angle; };
    void rotateZ(float angle) { rotation.z += angle; };

private:
    std::string modelName;
    std::vector<Mesh> meshes;
    glm::mat4 model;
    glm::vec3 position;
    glm::vec3 rotation;

    std::shared_ptr<std::vector<Texture>> textures;
};

#endif