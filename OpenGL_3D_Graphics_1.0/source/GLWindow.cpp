//
// Created by Harry Wang on 2024-12-13.
//
#include <GLWindow.h>

#include "ShapeBuilder.h"

GLWindow::GLWindow() {

}

GLWindow::~GLWindow() {

}


void GLWindow::createVBO(GLuint size, Vertex* vertexDataPtr = nullptr) {
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    if(vertexDataPtr != nullptr)
        glBufferData(GL_ARRAY_BUFFER, size, (void*)data, GL_STATIC_DRAW);
}

void GLWindow::createEBO(GLuint size, GLuint* indexDataPtr = nullptr) {
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    if(indexDataPtr != nullptr)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, (void*)indexDataPtr,GL_STATIC_DRAW);
}

