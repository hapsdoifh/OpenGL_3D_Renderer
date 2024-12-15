//
// Created by Harry Wang on 2024-12-13.
//

#include "ShapeBuilder.h"

//dynamically allocated memory, need to be deleted
//TODO: make the memory cleanup local to this class
void ShapeBuilder::buildCube() {
    Vertex cubeVerts[] {
        glm::vec3(-1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f)
    };
    numVertices = sizeof(cubeVerts)/sizeof(cubeVerts[0]);
    vertexData = new Vertex[numVertices];
    vertexByteSize = sizeof(cubeVerts);
    //TODO: use alternative copy method?
    memcpy(vertexData, cubeVerts, vertexByteSize);

    GLuint cubeInds[] {
        0,1,2,
        2,1,3
    };

    numIndices = sizeof(cubeInds)/sizeof(cubeInds[0]);
    indexData = new GLuint[numIndices];
    indexByteSize = sizeof(cubeInds);
    memcpy(indexData, cubeInds, indexByteSize);
}

ShapeBuilder::ShapeBuilder() {

}

ShapeBuilder::~ShapeBuilder() {
    delete[] vertexData;
    delete[] indexData;
}

