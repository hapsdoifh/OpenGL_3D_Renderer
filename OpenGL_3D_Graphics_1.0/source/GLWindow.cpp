//
// Created by Harry Wang on 2024-12-13.
//
#include <GLWindow.h>
#include "ShapeBuilder.h"
#include <unistd.h> //for debug
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

GLuint GLWindow::createVAO() {
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
}


bool GLWindow::readShaderFile(const std::string &path, std::string &dest) {
    char cwd[100];
    if(getcwd(cwd, 100) != nullptr) {
        std::cout << cwd << std::endl;
    }
    std::string relative_path = "/Users/harrywang/Projects/OpenGL_3D_Renderer/OpenGL_3D_Graphics_1.0/source/" + path;
    std::ifstream shaderReader(relative_path, std::ios::in);
    if(shaderReader.is_open()) {
        dest = {std::istreambuf_iterator<char>(shaderReader), std::istreambuf_iterator<char>()};
        return true;
    }

    return false;
}

void GLWindow::setVertexAttribPtr(GLuint attribLayoutLoc, GLint attribSize, GLint stride, int offset, GLenum dataType, GLenum normalized) {
    glEnableVertexAttribArray(attribLayoutLoc);
    glVertexAttribPointer(attribLayoutLoc, attribSize, dataType, normalized, stride, (void*)offset);
}


void GLWindow::createShaders() {
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
}

void GLWindow::compileShaders() {
    compileShader(vertexShaderID,"VertexShaderCode.glsl");
    compileShader(fragmentShaderID,"FragmentShaderCode.glsl");
}

GLuint GLWindow::compileShader(GLuint shaderID, std::string shaderPath) {
    //load shaders
    std::string shaderCode;
    if(!readShaderFile(shaderPath, shaderCode)) {
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
        exit(0);
    }
}
