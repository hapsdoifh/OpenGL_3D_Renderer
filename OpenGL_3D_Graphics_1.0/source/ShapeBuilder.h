//
// Created by Harry Wang on 2024-12-13.
//

#ifndef SHAPEBUILDER_H
#define SHAPEBUILDER_H
#include <glm/glm.hpp>
#include <glad/glad.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texture;
    glm::vec3 normal;
};

class ShapeBuilder {

public:
    GLuint numVertices;
    GLuint numIndices;
    GLuint byteSize;
    ShapeBuilder();
    void buildCube();
    void importData(); //FUTURE TODO
};



#endif //SHAPEBUILDER_H
