//
// Created by Harry Wang on 2024-12-13.
//

#include "ShapeBuilder.h"
// #include <iostream>

//dynamically allocated memory, need to be deleted
//TODO: make the memory cleanup local to this class
// void ShapeBuilder::buildCube() {
//     Vertex cubeVerts[] {
//         glm::vec3(-1.0f, 1.0f, 0.0f),
//         glm::vec3(1.0f, 1.0f, 0.0f),
//         glm::vec3(-1.0f, -1.0f, 0.0f),
//         glm::vec3(1.0f, -1.0f, 0.0f)
//     };
//     numVertices = sizeof(cubeVerts)/sizeof(cubeVerts[0]);
//     vertexData = new Vertex[numVertices];
//     vertexByteSize = sizeof(cubeVerts);
//     //TODO: use alternative copy method?
//     memcpy(vertexData, cubeVerts, vertexByteSize);
//
//     GLuint cubeInds[] {
//         0,1,2,
//         2,1,3
//     };
//
//     numIndices = sizeof(cubeInds)/sizeof(cubeInds[0]);
//     indexData = new GLuint[numIndices];
//     indexByteSize = sizeof(cubeInds);
//     memcpy(indexData, cubeInds, indexByteSize);
// }

void ShapeBuilder::buildCube(GLfloat sideLengthScale, glm::vec3 color) {

    // for(int i{0}; i<8; i++) {
    //     vertexData[i] = vertexData[0];
    // }

    Vertex cubeVerts[] {
        sideLengthScale * glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0,0.0,0.0),

        sideLengthScale * glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0,0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0,0.0,0.0)
    };

    // GLuint cubeInds[] {
    //     0,1,2,
    //     2,1,3,
    //
    // };

    GLuint cubeInds[]{
        0,1,2, 2,1,3,
        0,4,1, 1,4,5,
        4,6,5, 5,6,7,
        2,3,6, 6,3,7,
        0,2,4, 4,2,6,
        1,5,3, 3,5,7,
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
        glm::vec3 faceNormal = glm::normalize(glm::cross(edgeVecB, edgeVecA)); // weight each face equally, is optional

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
    for(int i{0}; i < sizeof(indexData); i++)
        indexData[i] = i;
}

ShapeBuilder::ShapeBuilder() {

}

ShapeBuilder::~ShapeBuilder() {
    // delete[] vertexData;
    // delete[] indexData;
}