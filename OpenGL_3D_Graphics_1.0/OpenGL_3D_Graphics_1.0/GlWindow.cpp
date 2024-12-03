

#include <gl/glew.h>
#include "GlWindow.h"
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <Vertex.h>
#include <ShapeGenerator.h>

using glm::mat4;
using glm::vec3;

const uint MAX_TRI = 100;
const uint VERT_PER_TRI = 3;
const uint FLOAT_PER_VERT = 3;
const uint TRI_BYTE_SIZE = VERT_PER_TRI * 6 * sizeof(GLfloat);


ShapeData myShape = ShapeGenerator::makeCube();

GLuint programID;

GlWindow::~GlWindow() {
	glUseProgram(0); //stop using the program
	glDeleteProgram(programID);
}

void GlWindow::sendDataToOpenGL() {
	

	GLuint myBUFFID;
	glGenBuffers(1, &myBUFFID); //creates a object-like buffer
	glBindBuffer(GL_ARRAY_BUFFER, myBUFFID); //binds buffer to array_buffer binding point
	glBufferData(GL_ARRAY_BUFFER, myShape.vertexBufferSize(), myShape.vertices, GL_STATIC_DRAW);  // Sends buffer data down to gpu
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6 /*start of one vert data to another*/, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (char*)(sizeof(GL_FLOAT) * 3));

	//GLushort because models generally have ubyte < size < uint number of indices and ushort is a nice fit
	//GLushort indices[] = { 0,1,2,3,4,5 };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, myShape.indexBufferSize(), myShape.indices, GL_STATIC_DRAW);
	
	myShape.cleanup();
}

bool GlWindow::checkShaderStatus(GLuint ShaderID) {
	GLint compileStatus;
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &compileStatus); //iv -> integer vector(array)

	if (compileStatus != GL_TRUE) {
		GLint infoLogLength;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* buffer = new GLchar[infoLogLength];
		GLsizei actualBuffSize;
		glGetShaderInfoLog(ShaderID, infoLogLength, &actualBuffSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool GlWindow::checkProgramStatus(GLuint programID) {
	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus); //iv -> integer vector(array)
	if (linkStatus != GL_TRUE) {
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar* buffer = new GLchar[infoLogLength];
		GLsizei actualBuffSize;
		glGetProgramInfoLog(programID, infoLogLength, &actualBuffSize, buffer);
		std::cout << buffer << std::endl;
		delete[] buffer;
		return false;
	}
	return true;
}

std::string GlWindow::readShaderFile(const char* fileName) {
	std::ifstream inputStream(fileName);
	if (!inputStream.good()) {
		std::cout << "failed to load " << fileName;
		exit(1);
	}
	return std::string(std::istreambuf_iterator<char>(inputStream), std::istreambuf_iterator<char>());
}

void GlWindow::installShaders() {
	//Vertex Shader using GLSL
	//Fragement Shader using GLSL
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER); //creates a object-like shader
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER); //creates a object-like shader

	const char* adapter[1];
	std::string temp = readShaderFile("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderFile("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(vertexShaderID)) {
		return;
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	//links the shader programs?
	glLinkProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	
	if (!checkProgramStatus(programID))
		return;
	

	glUseProgram(programID);
}


int numTris = 0;
void GlWindow::sendTriOpenGL() {
	const GLfloat x_delt = 0.1f;
	const GLfloat curr_x = -1.0f + x_delt * numTris;
	if (numTris >= MAX_TRI)
		return;
	Vertex thisTri[]{
		glm::vec3(curr_x, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(curr_x + x_delt, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(curr_x, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
	};
	glBufferSubData(GL_ARRAY_BUFFER, numTris * TRI_BYTE_SIZE, TRI_BYTE_SIZE, thisTri);
	numTris++;
}

void GlWindow::initializeGL() {
	glewInit();
	glEnable(GL_DEPTH_TEST); 
	sendDataToOpenGL();
	installShaders(); 
}

void GlWindow::paintGL() {
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_DEPTH_BUFFER);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	//glm::vec3 domColor(1.0f, 1.0f, 0.0f);
	//GLint domColorUniLocation = glGetUniformLocation(programID, "domColor");
	//glUniform3fv(domColorUniLocation, 1, &domColor[0]);
	GLint fullMtxLoc = glGetUniformLocation(programID, "fullTransformationMatrix");

	mat4 projectionMatrix = glm::perspective(glm::radians(80.0f), ((float)width()) / height(), 0.1f, 10.0f); //project front plane close, far plane way far
	mat4 modelTranslateMatrix = glm::translate(projectionMatrix, vec3(-1.0f, 0.0f, -3.0f));
	mat4 fullTranformMatrix = glm::rotate(modelTranslateMatrix, glm::radians(36.0f), vec3(1.0f, 0.0f, 0.0f));
	
	glUniformMatrix4fv(fullMtxLoc, 1, GL_FALSE, &fullTranformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, myShape.numIndices, GL_UNSIGNED_SHORT, 0);
	 
	//cube 2
	modelTranslateMatrix = glm::translate(projectionMatrix, vec3(1.0f, -0.0f, -3.75f));
	fullTranformMatrix = glm::rotate(modelTranslateMatrix, glm::radians(126.0f), vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(fullMtxLoc, 1, GL_FALSE, &fullTranformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, myShape.numIndices, GL_UNSIGNED_SHORT, 0);

	//sendTriOpenGL();
	//std::cout << numTris << std::endl;
	//glDrawArrays(GL_TRIANGLES, 0, 3);
}