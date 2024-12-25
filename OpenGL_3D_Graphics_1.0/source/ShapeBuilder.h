//
// Created by Harry Wang on 2024-12-13.
//

#ifndef SHAPEBUILDER_H
#define SHAPEBUILDER_H
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <vector>

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::mat3;
using glm::vec4;

struct Vertex {
    glm::vec3 position;
    // glm::vec3 color;
    // glm::vec2 texture;
    glm::vec3 normal;
};

class ShapeBuilder {

public:
    ShapeBuilder();
    ~ShapeBuilder();
    GLuint numVertices;
    GLuint vertexByteSize;
    GLuint numIndices;
    GLuint indexByteSize;
    Vertex* vertexData;
    GLuint* indexData;
    void buildCube(GLfloat sideLengthScale=1.0f, glm::vec3 color=glm::vec3(0,0,0));
    void buildNormals(ShapeBuilder& srcShape);
    void importData(); //FUTURE TODO
    void cleanUP();
};

#endif //SHAPEBUILDER_H
