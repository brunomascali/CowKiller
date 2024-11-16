#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <mesh.hpp>
#include <GLM/mat4x4.hpp>
#include <shader_program.hpp>

class Model {
public:
    Model(const char* filename);
    void createMeshes(const aiScene* scene, const aiNode* node);

    void render(ShaderProgram &shader);

    void rotateX(float angle) { rotation.x += angle; };
    void rotateY(float angle) { rotation.y += angle; };
    void rotateZ(float angle) { rotation.z += angle; };

private:
    std::vector<Mesh> meshes;
    glm::mat4 model;
    glm::vec3 rotation;
};

#endif