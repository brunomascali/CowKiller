#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <filesystem>
#include <mesh.hpp>
#include <GLM/mat4x4.hpp>
#include <shader_program.hpp>
#include <unordered_map>
#include <texture.hpp>

class Model {
public: 
    Model(std::filesystem::path modelPath);
    void createMesh(const aiScene* scene, const aiNode* node);

    void render(ShaderProgram &shader);
    
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