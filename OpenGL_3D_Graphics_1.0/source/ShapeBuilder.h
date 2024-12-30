//
// Created by Harry Wang on 2024-12-13.
//

#ifndef SHAPEBUILDER_H
#define SHAPEBUILDER_H
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <fstream>

using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::mat3;
using glm::vec4;

using std::string;
using std::vector;

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 texture;
};

class ShapeBuilder {

public:
    ShapeBuilder();
    ~ShapeBuilder();
    GLuint vaoIndex;
    GLuint numVertices;
    GLuint vertexByteSize;
    GLuint numIndices;
    GLuint indexByteSize;
    Vertex* vertexData;
    GLuint* indexData;
    void buildCube(GLfloat sideLengthScale=1.0f, glm::vec3 color=glm::vec3(1.0f,1.0f,1.0f));
    void buildNormals(ShapeBuilder& srcShape);
    std::vector<std::string> splitFileLine(std::string fileLine);
    void fileImportParallel(string& fileStr, long start, long end, std::string newLine,
        std::vector<vec3>& destVert, std::vector<vec3>& destNorm, vector<vec2> &destTex, std::vector<std::vector<std::string>>& destFace);
    void importShape(std::string path);
    void calcVertexNorm();
    void cleanUP();
};

#endif //SHAPEBUILDER_H
