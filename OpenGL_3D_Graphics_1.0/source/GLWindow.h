#pragma once
#include <glm/glm.hpp>
#include <glad.h>
#include <ShapeBuilder.h>
#include <iostream>

class GLWindow{

	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	GLuint vertexBufferID;
	GLuint indexBufferID;

	bool readShaderFile(const std::string &path, std::string &dest);
public:
	GLWindow();
	~GLWindow();

	void createVBO(GLuint size, Vertex* vertexData); //Vertex Buffer Object
	void createEBO(GLuint, GLuint* indexData); //Element Buffer Object
	GLuint createVAO(); //Vertex Array Object => TODO

	GLuint compileShader(GLuint shaderID);
	void creatProgram();

};