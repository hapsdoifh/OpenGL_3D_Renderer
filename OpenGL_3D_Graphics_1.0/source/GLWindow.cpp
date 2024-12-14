//
// Created by Harry Wang on 2024-12-13.
//
#include <GLWindow.h>
#include "ShapeBuilder.h"
#include <fstream>

GLWindow::GLWindow() {

}

GLWindow::~GLWindow() {

}


void GLWindow::createVBO(GLuint size, Vertex* vertexDataPtr = nullptr) {
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    if(vertexDataPtr != nullptr)
        glBufferData(GL_ARRAY_BUFFER, size, (void*)vertexDataPtr, GL_STATIC_DRAW);
}

void GLWindow::createEBO(GLuint size, GLuint* indexDataPtr = nullptr) {
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    if(indexDataPtr != nullptr) //send data if there is any
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, (void*)indexDataPtr,GL_STATIC_DRAW);
}

bool GLWindow::readShaderFile(const std::string &path, std::string &dest) {
    std::ifstream shaderReader(path, std::ios::in);
    if(shaderReader.is_open()) {
        dest = {std::istreambuf_iterator<char>(), std::istreambuf_iterator<char>()};
        return true;
    }

    return false;
}


GLuint GLWindow::compileShader(GLuint shaderID) {
    //load shaders
    std::string shaderCode;
    if(!readShaderFile("VertexShaderCode.glsl", shaderCode)) {
        std::cout << "Error when reading Vertex Shader, exiting";
        exit(0);
    }
    const char* const codePtr = shaderCode.c_str();
    glShaderSource(shaderID, 1, &codePtr, nullptr); //This might have issues with accessing the "array" of const char pointers
    glCompileShader(shaderID);

    GLint compileResult;
    GLint logLength;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
    if(compileResult == GL_FALSE) {
        std::vector<char> compileLogMsg(logLength + 1); //Probably null terminated? need to check
        glGetShaderInfoLog(shaderID, logLength, NULL, &compileLogMsg[0]);
        std::cout << std::string(compileLogMsg.begin(), compileLogMsg.end());
    }
    return shaderID;
}

void GLWindow::creatProgram() {
    programID = glCreateProgram();
    //requires both shaders to be initialized at this point
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    GLint linkResult;
    GLint logLength;
    glGetProgramiv(programID,GL_LINK_STATUS, &linkResult);
    glGetProgramiv(programID,GL_INFO_LOG_LENGTH, &logLength);
    if(linkResult==GL_FALSE) {
        std::vector<char> linkMsgLog(logLength + 1);
        glGetProgramInfoLog(programID, logLength, NULL, &linkMsgLog[1]);
        std::cout << std::string(linkMsgLog.begin(), linkMsgLog.end());
    }
}
