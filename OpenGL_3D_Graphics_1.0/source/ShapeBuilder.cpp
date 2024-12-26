//
// Created by Harry Wang on 2024-12-13.
//

#include "ShapeBuilder.h"
#include <regex>
#include <unistd.h>

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

std::vector<std::string> ShapeBuilder::splitFileLine(std::string fileLine) {
    std::vector<std::string> fileList;
    std::string processedLine = std::regex_replace(fileLine, std::regex("/"), " ");
    processedLine = std::regex_replace(processedLine, std::regex("  "), " ");
    while(processedLine.find(" ") != std::string::npos) {
        fileList.push_back(processedLine.substr(0, processedLine.find(" ")));
        processedLine.erase(processedLine.begin(), processedLine.begin() + processedLine.find(" ") + 1);
    }
    if(!processedLine.empty())
        fileList.push_back(processedLine.substr(0,processedLine.size()));
    return fileList;
}

void ShapeBuilder::importShape(std::string path) {
    std::vector<glm::vec3> vertexList;
    std::vector<glm::vec3> normalList;
    std::vector<std::vector<string>> faceList;
    std::vector<Vertex> tempVertList;
    std::vector<GLuint> tempIndList;

    char cwd[100];
    if(getcwd(cwd, 100) != nullptr)
        std::cout << cwd << std::endl;

    std::string curPath = cwd;
    std::string relative_path = curPath + "/../OpenGL_3D_Graphics_1.0/source/" + path;
    std::ifstream shapeFile(relative_path, std::ios::in);
    if(!shapeFile.is_open()) {
        std::cout << "Cannot open shape file, exitting";
        exit(0);
    }
    std::string fileLine;
    while(std::getline(shapeFile, fileLine)) {
        std::vector<std::string> fileList = splitFileLine(fileLine);
        if(fileList[0] == "v" ) {
            vertexList.push_back(vec3(std::stof(fileList[1]),std::stof(fileList[2]),std::stof(fileList[3])));
        }else if(fileList[0] == "vn") {
            normalList.push_back(vec3(std::stof(fileList[1]),std::stof(fileList[2]),std::stof(fileList[3])));
        }else if(fileList[0] == "f") {
            faceList.push_back(fileList);
        }
    }
    for(auto& it : faceList) {
        std::vector<int> vertOrder = {0,1,2,0,2,3};
        if(it.size() <= 10)
            vertOrder = std::vector<int>(vertOrder.begin(), vertOrder.begin()+3);
        for(auto& i : vertOrder) {
            //from what I understand the face portion of .obj files specifies the vertex as well as the vertex normals it wants to use for a face
            //however in OpenGL every normal is tied with every vertex, meaning if I want different vertex normals I need to create whole different vertices
            //this makes the indexing useless as I have to use every vertex I created anyway. I'm just adding it to keep the style consistent
            Vertex tempVert;
            int vertInd = std::stoi(it[i*3 + 1]);
            //TODO process texture fileList[i+1]
            int normInd = std::stoi(it[i*3 + 3]);
            tempVert.position = vertexList[vertInd - 1];
            tempVert.normal = vertexList[normInd - 1];

            tempVert.color = vec3(0.0,0.0,1.0f);
            tempVertList.push_back(tempVert);
        }
    }
    delete[] vertexData;
    delete[] indexData; //just to be safe

    numVertices = tempVertList.size();
    vertexByteSize = numVertices * sizeof(Vertex);
    vertexData = new Vertex[tempVertList.size()];
    memcpy(vertexData, &tempVertList[0], tempVertList.size() * sizeof(Vertex));

    numIndices = numVertices;
    indexByteSize = numIndices * sizeof(GLuint);
    indexData = new GLuint[numIndices];
    for(int i{0}; i < numIndices; i++) {
        indexData[i] = i; //just filler to keep the consistent glDrawElement method
    }
}

ShapeBuilder::ShapeBuilder():
vertexData(nullptr),
indexData(nullptr)
{

}

ShapeBuilder::~ShapeBuilder() {
}