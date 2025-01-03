//
// Created by Harry Wang on 2024-12-13.
//

#include "ShapeBuilder.h"

#include <complex>
#include <regex>
#include <unistd.h>
#include <thread>
#include <stb_image.h>

void ShapeBuilder::buildCube(GLfloat sideLengthScale, glm::vec3 color) {

    //This is still a right-handed coordinate system so 1 will be closer than -1 once loaded in (in -z no rotation)
    Vertex cubeVerts[] {
        sideLengthScale * glm::vec3(-1.0f, 1.0f, 1.0f), color, glm::vec3(0.0,0.0,0.0), glm::vec2(0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, 1.0f, 1.0f), color, glm::vec3(0.0,0.0,0.0), glm::vec2(0.0,0.0),
        sideLengthScale * glm::vec3(-1.0f, -1.0f,1.0f), color, glm::vec3(0.0,0.0,0.0), glm::vec2(0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, -1.0f, 1.0f), color, glm::vec3(0.0,0.0,0.0), glm::vec2(0.0,0.0),

        sideLengthScale * glm::vec3(-1.0f, 1.0f, -1.0f), color, glm::vec3(0.0,0.0,0.0), glm::vec2(0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, 1.0f, -1.0f), color, glm::vec3(0.0,0.0,0.0), glm::vec2(0.0,0.0),
        sideLengthScale * glm::vec3(-1.0f, -1.0f, -1.0f), color, glm::vec3(0.0,0.0,0.0), glm::vec2(0.0,0.0),
        sideLengthScale * glm::vec3(1.0f, -1.0f, -1.0f), color, glm::vec3(0.0,0.0,0.0),  glm::vec2(0.0,0.0)
    };


    GLuint cubeInds[]{
        0,2,1, 1,2,3,
        0,1,4, 1,5,4,
        4,7,6, 5,7,4,
        2,7,3, 6,7,2,
        0,4,2, 4,6,2,
        1,7,5, 1,3,7,
    };

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
    calcVertexNorm();
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

vector<std::string> ShapeBuilder::splitFileLine(std::string fileLine) {
    vector<std::string> fileList;
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

void ShapeBuilder::fileImportParallel(std::string &fileStr, long start, long end, std::string newLine,
    vector<vec3> &destVert, vector<vec3> &destNorm, vector<vec2> &destTex, vector<vector<std::string>> &destFace) {
    long pos{start};
    while(pos < end) {
        long lineEnd;
        std::basic_string<char>::size_type findResult = fileStr.find(newLine, pos);
        if(findResult == std::string::npos)
            lineEnd = end;
        else
            lineEnd = static_cast<long>(findResult);

        std::string fileLine = fileStr.substr(pos, lineEnd - pos);
        vector<std::string> fileList = splitFileLine(fileLine);
        pos = lineEnd + newLine.size();
        if(fileList.empty())
            continue;
        if(fileList[0] == "v" ) {
            destVert.push_back(vec3(std::stof(fileList[1]),std::stof(fileList[2]),std::stof(fileList[3])));
        }else if(fileList[0] == "vn") {
            destNorm.push_back(vec3(std::stof(fileList[1]),std::stof(fileList[2]),std::stof(fileList[3])));
        }else if(fileList[0] == "vt") {
            destTex.push_back(vec2(std::stof(fileList[1]),1.0f - std::stof(fileList[2])));
        }else if(fileList[0] == "f") {
            destFace.push_back(fileList);
        }
    }
}


void ShapeBuilder::importShape(std::string path) {
    vector<glm::vec3> vertexList;
    vector<glm::vec3> normalList;
    vector<glm::vec2> texList;
    vector<vector<string>> faceList;
    vector<Vertex> tempVertList;
    vector<GLuint> tempIndList;

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
    std::string entireFile((std::istreambuf_iterator<char>(shapeFile)), std::istreambuf_iterator<char>());
    std::string newLine = "\n";
    if(entireFile.find("\r\n") != std::string::npos)
        newLine = "\r\n";
    std::cout << "done search of file for newline character, it is:" << newLine.size() << std::endl;

    int threadCount = 16;
    long chunkSize = entireFile.size() / threadCount;
    vector<long> startLoc(threadCount + 1, 0);
    vector<std::thread> readerThreads(threadCount);

    vector<vector<glm::vec3>> vertexChunkList(threadCount);
    vector<vector<glm::vec3>> normalChunkList(threadCount);
    vector<vector<glm::vec2>> texChunkList(threadCount);
    vector<vector<vector<string>>> faceChunkList(threadCount);
    for(int i{1}; i <= threadCount; i++) {
        startLoc[i] = chunkSize * i;

        if(i < threadCount) {
            long nextLine = entireFile.find(newLine,startLoc[i]);
            startLoc[i] = nextLine + newLine.size();
        }else {
            startLoc[i] = entireFile.size();
        }

        readerThreads[i-1] = std::thread(&ShapeBuilder::fileImportParallel, this, std::ref(entireFile), startLoc[i - 1], startLoc[i], newLine,
            std::ref(vertexChunkList[i - 1]), std::ref(normalChunkList[i - 1]), std::ref(texChunkList[i-1]), std::ref(faceChunkList[i - 1]));
    }
    for(auto& threadIt : readerThreads) {
        threadIt.join();
    }
    for(int i{0}; i<threadCount; i++) {
        vertexList.insert(vertexList.end(),vertexChunkList[i].begin(),vertexChunkList[i].end());
        normalList.insert(normalList.end(),normalChunkList[i].begin(),normalChunkList[i].end());
        texList.insert(texList.end(),texChunkList[i].begin(),texChunkList[i].end());
        faceList.insert(faceList.end(),faceChunkList[i].begin(),faceChunkList[i].end());
    }

    std::cout << "completed position and vertex import" << std::endl;
    for(vector<string>& it : faceList) {
        vector<int> vertOrder = {0,1,2,0,2,3};
        if(it.size() <= 10)
            vertOrder = vector<int>(vertOrder.begin(), vertOrder.begin()+3);
        for(int i : vertOrder) {
            //from what I understand the face portion of .obj files specifies the vertex as well as the vertex normals it wants to use for a face
            //however in OpenGL every normal is tied with every vertex, meaning if I want different vertex normals I need to create whole different vertices
            //this makes the indexing useless as I have to use every vertex I created anyway. I'm just adding it to keep the style consistent
            //size 6,8 => no texture eg: v/vn | size 9,12 => has texture eg: v/vt/vn (very basic checking)
            Vertex tempVert;
            int stride = 3;
            if((it.size() - 1) == 6 || (it.size() - 1) == 8) //minus the first symbol
                stride = 2;
            int vertInd = std::stoi(it[i*stride + 1]);
            int texInd = std::stoi(it[i*stride + 2]);
            int normInd = std::stoi(it[i*stride + stride]);
            tempVert.position = vertexList[vertInd - 1];
            tempVert.normal = glm::vec3(0.0,0.0,0.0);
            tempVert.texture = glm::vec2(0.0,0.0);

            if(!normalList.empty())
                tempVert.normal = normalList[normInd - 1];
            if(!texList.empty())
                tempVert.texture = texList[texInd - 1];

            tempVert.color = vec3(0.0,0.0,0.0f);
            tempVertList.push_back(tempVert);
        }
    }
    std::cout << "completed normal import" << std::endl;
    delete[] vertexData;
    delete[] indexData; //just to be safe

    numVertices = tempVertList.size();
    vertexByteSize = numVertices * sizeof(Vertex);
    vertexData = new Vertex[numVertices];
    memcpy(vertexData, &tempVertList[0], tempVertList.size() * sizeof(Vertex));

    numIndices = numVertices;
    indexByteSize = numIndices * sizeof(GLuint);
    indexData = new GLuint[numIndices];
    for(int i{0}; i < numIndices; i++) {
        indexData[i] = i; //just filler to keep the consistent glDrawElement method
    }
    //manually calculate vertex norm because it's not given in the file
    if(normalList.empty()) {
        calcVertexNorm();
    }
}

void ShapeBuilder::calcVertexNorm() {
    //cycle through all the faces and calculate face normals, add normal to contributing to vertices of that face
    for(GLuint faceIt{0}; faceIt < numIndices; faceIt += 3) {
        glm::vec3 edgeVecA, edgeVecB;
        GLuint currVertex = indexData[faceIt];
        GLuint currVertexA = indexData[faceIt+1];
        GLuint currVertexB = indexData[faceIt+2];
        edgeVecA = vertexData[currVertexA].position - vertexData[currVertex].position;
        edgeVecB = vertexData[currVertexB].position - vertexData[currVertexA].position;
        glm::vec3 faceNormal = glm::normalize(glm::cross(edgeVecA, edgeVecB)); // weight each face equally, is optional

        vertexData[currVertex].normal += faceNormal;
        vertexData[currVertexA].normal += faceNormal;
        vertexData[currVertexB].normal += faceNormal;
    }

    for(int i{0}; i < numVertices; i++) {
        vertexData[i].normal = glm::normalize(vertexData[i].normal);
    }

}


ShapeBuilder::ShapeBuilder():
vertexData(nullptr),
indexData(nullptr),
numVertices(0),
numIndices(0),
vertexByteSize(0),
indexByteSize(0)
{

}

ShapeBuilder::~ShapeBuilder() {
    delete[] vertexData;
    vertexData = nullptr;
    delete[] indexData;
    indexData = nullptr;
}