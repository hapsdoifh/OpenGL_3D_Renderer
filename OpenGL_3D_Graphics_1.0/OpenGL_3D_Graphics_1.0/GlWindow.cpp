

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
#include <Camera.h>
#include <Qt3DInput/qmouseevent.h>
#include <QtGui/qevent.h>
#include <stb_image.h>


using glm::mat4;
using glm::vec3;

const uint MAX_TRI = 100;
const uint VERT_PER_TRI = 3;
const uint FLOAT_PER_VERT = 3;
const uint TRI_BYTE_SIZE = VERT_PER_TRI * 6 * sizeof(GLfloat);
Camera myCamera;

ShapeData myShape = ShapeGenerator::makeCube();

GLuint programID;
GLuint cubeVertexArrayObjID;
GLuint CubeVertBuffID;
GLuint cubeIndexBufferID;

GlWindow::~GlWindow() {
	glUseProgram(0); //stop using the program
	glDeleteProgram(programID);
}

void GlWindow::setupVertexArrays() {
	glGenVertexArrays(1, &cubeVertexArrayObjID);

	glBindVertexArray(cubeVertexArrayObjID);
	//all the buffer states from this point will be stored in the vertex array object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeVertBuffID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (char*)(sizeof(GL_FLOAT) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBufferID); 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);	
}

GLuint texture;
void GlWindow::sendDataToOpenGL() { //basically sending the model data down to opengl
	

	glGenBuffers(1, &CubeVertBuffID); //creates a object-like buffer
	glBindBuffer(GL_ARRAY_BUFFER, CubeVertBuffID); //binds buffer to array_buffer binding point
	glBufferData(GL_ARRAY_BUFFER, myShape.vertexBufferSize(), myShape.vertices, GL_STATIC_DRAW);  // Sends buffer data down to gpu
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8 /*start of one vert data to another*/, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (char*)(sizeof(GL_FLOAT) * 3));

	//GLushort because models generally have ubyte < size < uint number of indices and ushort is a nice fit
	//GLushort indices[] = { 0,1,2,3,4,5 };
	glGenBuffers(1, &cubeIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, myShape.indexBufferSize(), myShape.indices, GL_STATIC_DRAW);

	myShape.cleanup();

	//GLuint TransformMatrixBuffID;
	//glGenBuffers(1, &TransformMatrixBuffID);
	//glBindBuffer(GL_ARRAY_BUFFER, TransformMatrixBuffID);

	//Textures

	int widthImg, heightImg, numColorCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("Gaben.png", &widthImg, &heightImg, &numColorCh, 0);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (char*)(sizeof(GL_FLOAT) * 6));

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	setMouseTracking(true);
	glewInit();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	sendDataToOpenGL();
	//setupVertexArrays();
	installShaders(); 
}


void GlWindow::mouseMoveEvent(QMouseEvent* event) {
	QPointF temp;
	temp = event->position();
	if(event->buttons() == Qt::LeftButton)
		myCamera.mouseUpdate(glm::vec2(temp.x(), temp.y()));
	update();
	//std::cout << "test";
	//paintGL();
}


void GlWindow::keyPressEvent(QKeyEvent* event) {
	GLfloat frontBack = 0;
	GLfloat leftRight = 0;
	GLfloat upDown = 0;
	switch (event->key()) {
	case Qt::Key::Key_W:
		frontBack = 1.0f;
		break;
	case Qt::Key::Key_S:
		frontBack = -1.0f;
		break;
	case Qt::Key::Key_A:
		leftRight = -1.0f;
		break;
	case Qt::Key::Key_D:
		leftRight = 1.0f;
		break;
	case Qt::Key::Key_Space:
		upDown = 1.0f;
		break;
	case Qt::Key::Key_C:
		upDown = -1.0f;
		break;
	}
	myCamera.keyboardUpdate(frontBack, leftRight, upDown);
	update();
}
void GlWindow::paintGL() {

	glViewport(0, 0, width(), height());
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_DEPTH_BUFFER);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(programID);
	GLuint text0Uni = glGetUniformLocation(programID, "tex0");
	glUniform1i(text0Uni, 0.5);
	glBindTexture(GL_TEXTURE_2D, texture);

	mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), ((float)width()) / height(), 0.1f, 100.0f); //project front plane close, far plane way far
	mat4 modelTranslateMatrix = glm::translate(projectionMatrix, vec3(-1.0f, 0.0f, -3.0f));
	mat4 fullTranformMatrix = glm::rotate(modelTranslateMatrix, glm::radians(36.0f), vec3(1.0f, 0.0f, 0.0f));

	mat4 fullTransforms[] = {
		projectionMatrix * myCamera.getWorldToViewMatrix() * glm::translate(mat4(1.0f), vec3(-1.0f, 0.0f, -3.0f)) * glm::rotate(mat4(1.0f), glm::radians(36.0f), vec3(1.0f, 0.0f, 0.0f)),
		projectionMatrix * myCamera.getWorldToViewMatrix() * glm::translate(mat4(1.0f), vec3(1.0f, 0.0f, -3.75f)) * glm::rotate(mat4(1.0f), glm::radians(126.0f), vec3(0.0f,1.0f, 0.0f))
	};
	//glBindVertexArray(cubeVertexArrayObjID);
	GLuint MatLoc = glGetUniformLocation(programID, "fullTransformationMatrix");
	glUniformMatrix4fv(MatLoc, 1, GL_FALSE, &fullTransforms[0][0][0]);
	//glUniformMatrix4fv(MatLoc, 1, GL_FALSE, &mat4(1.0f)[0][0]);
	glDrawElements(GL_TRIANGLES, myShape.numIndices, GL_UNSIGNED_SHORT, 0);

	//glBindVertexArray(cubeVertexArrayObjID);
	////MatLoc = glGetUniformLocation(programID, "fullTransformationMatrix");
	//glUniformMatrix4fv(MatLoc, 1, GL_FALSE, &fullTransforms[1][0][0]);
	//glDrawElements(GL_TRIANGLES, myShape.numIndices, GL_UNSIGNED_SHORT, 0);
	//glDrawElementsInstanced(GL_TRIANGLES, myShape.numIndices, GL_UNSIGNED_SHORT, 0, 2);

}