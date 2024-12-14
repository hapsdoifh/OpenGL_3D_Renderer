#pragma once
#include <glm/glm.hpp>
#include <glad.h>
#include <ShapeBuilder.h>

class GLWindow{

	GLuint programID;
	GLuint vertexBufferID;
	GLuint indexBufferID;

public:
	GLWindow();
	~GLWindow();

	void createVBO(GLuint size, Vertex* vertexData); //Vertex Buffer Object
	void createEBO(GLuint, GLuint* indexData); //Element Buffer Object
	GLuint createVAO(); //Vertex Array Object => TODO

	GLuint compileShaders();
	GLuint creatProgram();
};