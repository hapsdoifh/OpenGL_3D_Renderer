//
// Created by Harry Wang on 2024-12-13.
//
#include <GLWindow.h>
#include "ShapeBuilder.h"
#include <unistd.h> //for debug
#include <fstream>


int GLWindow::pollKey = 0;
vec3 GLWindow::pollKeyMovement(0.0f,0.0f,0.0f);

int GLWindow::pollMouseBtn = 0;
int GLWindow::pollMouseBtnAction = 0;

double GLWindow::pollMouseX = 0.0f;
double GLWindow::pollMouseY = 0.0f;

int GLWindow::pollUpdate = noUpdate;

using glm::vec3;

GLWindow::GLWindow() {

}

GLWindow::~GLWindow() {

}

void GLWindow::createVBO(GLuint size, Vertex* vertexDataPtr = nullptr) {
    GLuint tempVBO;
    glGenBuffers(1, &tempVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tempVBO);
    vertexBufferIDs.push_back(tempVBO);
    if(vertexDataPtr != nullptr)
        glBufferData(GL_ARRAY_BUFFER, size, (void*)vertexDataPtr, GL_STATIC_DRAW);
}

void GLWindow::createEBO(GLuint size, GLuint* indexDataPtr = nullptr) {
    GLuint tempEBO;
    glGenBuffers(1, &tempEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempEBO);
    indexBufferIDs.push_back(tempEBO);
    if(indexDataPtr != nullptr) //send data if there is any
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, (void*)indexDataPtr,GL_STATIC_DRAW);
}

void GLWindow::createVAO() {
    GLuint tempVAO;
    glGenVertexArrays(1, &tempVAO);
    glBindVertexArray(tempVAO);
    vertexArrayIDs.push_back(tempVAO);
}

void GLWindow::bindVAO(int index) {
    if (index < vertexArrayIDs.size()) {
        glBindVertexArray(vertexArrayIDs[index]);
    }else {
        std::cout << "VAO ID out of range, index is: " << index << std::endl;
        std::cout << "VAO list size is: " << vertexArrayIDs.size();
    }
}

void GLWindow::unbindVAO(int index) {
    if (index < vertexArrayIDs.size()) {
        glBindVertexArray(0);
    }else {
        std::cout << "VAO ID out of range, index is: " << index << std::endl;
        std::cout << "VAO list size is: " << vertexArrayIDs.size();
    }
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

void GLWindow::compileShaders(std::string vshaderPath, std::string fshaderPath) {
    compileShader(vertexShaderID,vshaderPath.c_str());
    compileShader(fragmentShaderID,fshaderPath.c_str());
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
    vec3 lightPos(0.0f,6.0f,0.0f);
    glUniform3fv(lightPosLoc, 1, &lightPos[0]);

    GLint normalMatLoc = glGetUniformLocation(programID, "normalRotateMat");
    glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, &mat3(rotateMat)[0][0]);

    return finalMat;
}

void GLWindow::handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }
    //pollKeyMovement: .x = strafe, .y = lookat, .z = UP
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) pollKeyMovement += 0.1f * vec3(1.0f,0.0f,0.0f);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) pollKeyMovement -= 0.1f * vec3(1.0f,0.0f,0.0f);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pollKeyMovement += 0.1f * vec3(0.0f,1.0f,0.0f);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pollKeyMovement -= 0.1f * vec3(0.0f,1.0f,0.0f);
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) pollKeyMovement += 0.1f * vec3(0.0f,0.0f,1.0f);
    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) pollKeyMovement -= 0.1f * vec3(1.0f,0.0f,1.0f);
    pollKey = key;
    pollUpdate |= keyUpdate;
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
    if(pollUpdate & keyUpdate) {
        myCam.cameraUpdateKeyboard(pollKey, pollKeyMovement);
        pollKeyMovement = vec3(0.0f,0.0f,0.0f);
    }
    if(pollUpdate & mouseUpdate)
        myCam.cameraUpdateMouse(pollMouseX, pollMouseY);
    if(pollUpdate & mouseBtnUpdate)
        myCam.cameraUpdateMouseBtn(pollMouseBtn, pollMouseBtnAction);
}
