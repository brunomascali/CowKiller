#ifndef MESH_CPP
#define MESH_CPP

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh {
public:
    Mesh(const aiMesh* mesh);

    void add_vertices(const aiMesh* mesh);
    void add_indices(const aiMesh* mesh);
    void add_normals(const aiMesh* mesh);

    void render() const;

    GLuint VAO;
    GLuint VBO_vertices;
    GLuint VBO_indices;
    GLuint VBO_normals;
    unsigned int numIndices;
private:

};

#endif