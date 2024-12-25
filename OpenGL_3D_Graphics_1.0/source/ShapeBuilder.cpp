//
// Created by Harry Wang on 2024-12-13.
//

#include "ShapeBuilder.h"
// #include <iostream>

void ShapeBuilder::buildCube(GLfloat sideLengthScale, glm::vec3 color) {

    //This is still a right-handed coordinate system so 1 will be closer than -1 once loaded in (in -z no rotation)
    Vertex cubeVerts[] {
        sideLengthScale * glm::vec3(-1.0f, 1.0f, 1.0f),  glm::vec3(0.0,0.5,0.0), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, 1.0f, 1.0f),  glm::vec3(0.0,0.5,0.0), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(-1.0f, -1.0f,1.0f),  glm::vec3(0.0,0.5,0.0), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, -1.0f, 1.0f),  glm::vec3(0.0,0.5,0.0), glm::vec3(0.0,0.0,0.0),

        sideLengthScale * glm::vec3(-1.0f, 1.0f, -1.0f),  glm::vec3(0.0,0.5,0.0), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, 1.0f, -1.0f),  glm::vec3(0.0,0.5,0.0), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(-1.0f, -1.0f, -1.0f),  glm::vec3(0.0,0.5,0.0), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec3(0.0,0.5,0.0), glm::vec3(0.0,0.0,0.0)
    };


    GLuint cubeInds[]{
        0,2,1, 1,2,3,
        0,1,4, 1,5,4,
        4,7,6, 5,7,4,
        2,7,3, 6,7,2,
        0,4,2, 4,6,2,
        1,7,5, 1,3,7,
    };
    //calculate vertex normals:
    std::vector<glm::vec3>faceNormals;
    for(GLuint faceIt{0}; faceIt < sizeof(cubeInds)/sizeof(GLuint); faceIt += 3) {
        glm::vec3 edgeVecA, edgeVecB;
        GLuint currVertex = cubeInds[faceIt];
        GLuint currVertexA = cubeInds[faceIt+1];
        GLuint currVertexB = cubeInds[faceIt+2];
        edgeVecA = cubeVerts[currVertexA].position - cubeVerts[currVertex].position;
        edgeVecB = cubeVerts[currVertexB].position - cubeVerts[currVertexA].position;
        glm::vec3 faceNormal = glm::normalize(glm::cross(edgeVecA, edgeVecB)); // weight each face equally, is optional

        cubeVerts[currVertex].normal += faceNormal;
        cubeVerts[currVertexA].normal += faceNormal;
        cubeVerts[currVertexB].normal += faceNormal;
    }

    for(Vertex &vertIt : cubeVerts) {
        vertIt.normal = glm::normalize(vertIt.normal);
    }

    //copy vertex data
    numVertices = sizeof(cubeVerts)/sizeof(cubeVerts[0]);
    vertexData = new Vertex[numVertices];
    vertexByteSize = sizeof(cubeVerts);
    //TODO: use alternative copy method?
    memcpy(vertexData, cubeVerts, vertexByteSize);

    //copy index data
    numIndices = sizeof(cubeInds)/sizeof(cubeInds[0]);
    indexData = new GLuint[numIndices];
    indexByteSize = sizeof(cubeInds);
    memcpy(indexData, cubeInds, indexByteSize);
}

void ShapeBuilder::buildNormals(ShapeBuilder &srcShape) {
    numVertices = srcShape.numVertices * 2;
    vertexByteSize = numVertices * sizeof(Vertex);
    vertexData = new Vertex[numVertices];
    int normVertCnt = 0;
    //the two points for the normal are the actual shape vertex itself + vertex normal end point
    for (int i{0}; i < srcShape.numVertices; i++) {
        vertexData[normVertCnt].position = srcShape.vertexData[i].position;
        vertexData[normVertCnt + 1].position = srcShape.vertexData[i].position + srcShape.vertexData[i].normal;
        normVertCnt += 2;
    }
    indexData = new GLuint[numIndices];
    for(int i{0}; i < numIndices; i++)
        indexData[i] = i;
}

void ShapeBuilder::cleanUP() {
    delete[] vertexData;
    vertexData = nullptr;
    delete[] indexData;
    indexData = nullptr;
}

ShapeBuilder::ShapeBuilder() {

}

ShapeBuilder::~ShapeBuilder() {
}