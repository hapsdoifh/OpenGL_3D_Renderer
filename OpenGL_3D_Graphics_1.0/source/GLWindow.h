#pragma once
#include <glm/glm.hpp>
#include <glad.h>

class GLWindow{
	
	
public:
	GLWindow();
	~GLWindow();

	GLuint createVBO(); //Vertex Buffer Object
	GLuint createEBO(); //Element Buffer Object
	GLuint createVAO(); //Vertex Array Object => TODO
	
	GLuint configBuffers();

	GLuint compileShaders();
	GLuint creatProgram();

};