#pragma once
#include <glm/glm.hpp>
#include <glad.h>
#include <ShapeBuilder.h>
#include <iostream>

class GLWindow{

	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	GLuint vertexBufferID;
	GLuint indexBufferID;

	GLuint vertexArrayID;

	static bool readShaderFile(const std::string &path, std::string &dest);
public:
	GLuint programID;

	GLWindow();
	~GLWindow();

	void createVBO(GLuint size, Vertex* vertexData); //Vertex Buffer Object
	void createEBO(GLuint, GLuint* indexData); //Element Buffer Object
	void createShaders();
	GLuint createVAO(); //Vertex Array Object => TODO

	void setVertexAttribPtr(GLuint attribLayoutLoc, GLint attribSize, GLint stride, int offset, GLenum dataType=GL_FLOAT, GLenum normalized=GL_FALSE);
	GLuint compileShader(GLuint shaderID, std::string shaderPath);
	void compileShaders();
	void creatProgram();

};