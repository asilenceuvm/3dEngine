#pragma once

#include <string>
#include <vector>

#include <gl/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct TextureStruct {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
private:
    unsigned int VBO, EBO;
    void setupMesh();

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureStruct> textures;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureStruct> textures);
    void render(Shader& shader);
};

