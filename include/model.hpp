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

    void render(const Shader& shader, CameraFree& camera, glm::vec3 translation, glm::vec3 rotation, float scalin);
    
    std::string modelName;
    std::vector<Mesh> meshes;

    std::shared_ptr<std::vector<Texture>> textures;
};

#endif