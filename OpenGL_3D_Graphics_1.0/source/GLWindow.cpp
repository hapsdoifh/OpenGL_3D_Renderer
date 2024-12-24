//
// Created by Harry Wang on 2024-12-13.
//
#include <GLWindow.h>
#include "ShapeBuilder.h"
#include <unistd.h> //for debug
#include <fstream>


int GLWindow::pollKey = 0;
int GLWindow::pollKeyAction = 0;

int GLWindow::pollMouseBtn = 0;
int GLWindow::pollMouseBtnAction = 0;

double GLWindow::pollMouseX = 0.0f;
double GLWindow::pollMouseY = 0.0f;

int GLWindow::pollUpdate = noUpdate;

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

void GLWindow::createVAO() {
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);
}


bool GLWindow::readShaderFile(const std::string &path, std::string &dest) {
    char cwd[100];
    if(getcwd(cwd, 100) != nullptr) {
        std::cout << cwd << std::endl;
    }
    std::string curPath = cwd;
    // std::string relative_path = curPath + "\\..\\OpenGL_3D_Graphics_1.0\\source\\" + path;
    std::string relative_path = curPath + "/../OpenGL_3D_Graphics_1.0/source/" + path;
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
    glUseProgram(programID);
}

mat4 GLWindow::sendFullMatrix(int width, int height) {
    //glm transformations follow normal conventions, eg: translate(A,B) = A*B

    //model (to world) => should it be done here? this is completed when placing models into the world
    mat4 translateMat = glm::translate(mat4(1.0f),vec3(0.0f,0.0f,-5.0f));
    mat4 rotateMat = glm::rotate(mat4(1.0f),glm::radians(30.0f),vec3(1.0f,0.0f,0.0f));

    //TODO:view (to view) => need camera class

    //projection (to projection)
    mat4 projMat = glm::perspective(glm::radians(60.0f), (float)width/height, 0.1f, 100.0f);
    mat4 viewMat = myCam.worldToCamMatrix();
    mat4 finalMat = projMat * viewMat * translateMat * rotateMat;

    GLint fullTransMatLoc = glGetUniformLocation(programID, "fullTransformMat");
    glUniformMatrix4fv(fullTransMatLoc, 1, GL_FALSE, &finalMat[0][0]);

    GLint lightPosLoc = glGetUniformLocation(programID, "lightPos");
    glm::vec3 lightPos(0.0f,6.0f,0.0f);
    glUniform3fv(lightPosLoc, 1, &lightPos[0]);

    GLint normalMatLoc = glGetUniformLocation(programID, "normalRotateMat");
    glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, &mat3(rotateMat)[0][0]);

    return finalMat;
}

void GLWindow::handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }else {
        pollKeyAction = action;
        pollKey = key;
        pollUpdate |= keyUpdate;
    }
}

void GLWindow::handleMouseCallback(GLFWwindow *window, double xpos, double ypos) {
    //TODO => Add mouse movement handling
    pollUpdate |= mouseUpdate;
    pollMouseX = xpos;
    pollMouseY = ypos;
}

void GLWindow::handleMouseBtnCallback(GLFWwindow *window, int button, int action, int mods) {
    pollUpdate |= mouseBtnUpdate;
    pollMouseBtn = button;
    pollMouseBtnAction = action;
}

void GLWindow::getPollingUpdate() {
    if(pollUpdate & keyUpdate)
        myCam.cameraUpdateKeyboard(pollKey, pollKeyAction);
    if(pollUpdate & mouseUpdate)
        myCam.cameraUpdateMouse(pollMouseX, pollMouseY);
    if(pollUpdate & mouseBtnUpdate)
        myCam.cameraUpdateMouseBtn(pollMouseBtn, pollMouseBtnAction);
}
