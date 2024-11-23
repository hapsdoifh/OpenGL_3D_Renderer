

#include <gl/glew.h>
#include "GlWindow.h"
#include <fstream>
#include <iostream>

void GlWindow::sendDataToOpenGL() {
	const float RED_TRIANGLE_Z = -0.5f, BLUE_TRIANGLE_Z = 0.5;
	
	GLfloat verts[] =
	{
		0.0f, -1.0f, RED_TRIANGLE_Z,
		1.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, RED_TRIANGLE_Z,
		1.0f, 0.0f, 0.0f,

		+1.0f, 1.0f, RED_TRIANGLE_Z,
		1.0f, 0.0f, 0.0f,

		-1.0f, -1.0f, BLUE_TRIANGLE_Z,
		0.0f, 0.0f, 1.0f,

		0.0f, 1.0f, BLUE_TRIANGLE_Z,
		0.0f, 0.0f, 1.0f,

		1.0f, -1.0f, BLUE_TRIANGLE_Z,
		0.0f, 0.0f, 1.0f,
	};
	GLuint myBUFFID;
	glGenBuffers(1, &myBUFFID); //creates a object-like buffer
	glBindBuffer(GL_ARRAY_BUFFER, myBUFFID); //binds buffer to array_buffer binding point
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);  // Sends buffer data down to gpu
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6 /*start of one vert data to another*/, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (char*)(sizeof(GL_FLOAT) * 3));

	//GLushort because models generally have ubyte < size < uint number of indices and ushort is a nice fit
	GLushort indices[] = { 0,1,2,3,4,5 };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	//links the shader programs?
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;
	

	glUseProgram(programID);
}

void GlWindow::initializeGL() {
	glewInit();
	glEnable(GL_DEPTH_TEST);
	sendDataToOpenGL();
	installShaders();
}


void GlWindow::paintGL() {
	//glClearColor(1, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glClear(GL_DEPTH_BUFFER);
	glViewport(0, 0, width(), height());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

}