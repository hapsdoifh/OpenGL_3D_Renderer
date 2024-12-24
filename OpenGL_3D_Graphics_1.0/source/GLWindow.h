#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glad.h>
#include <ShapeBuilder.h>
#include <iostream>
#include <glfw3.h>
#include <Camera.h>
#include <vector>

using glm::vec3;
using glm::mat3;
using glm::mat4;

enum updateType {
	noUpdate = 0b00,
	keyUpdate = 0b01,
	mouseUpdate = 0b10,
	mouseBtnUpdate = 0b100
};

class GLWindow{

	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	GLuint vertexBufferID;
	GLuint indexBufferID;

	GLuint vertexArrayID;

	static int pollKeyAction;
	static int pollKey;

	static int pollMouseBtnAction;
	static int pollMouseBtn;

	static double pollMouseX;
	static double pollMouseY;

	static int pollUpdate;

	Camera myCam;

	static bool readShaderFile(const std::string &path, std::string &dest);
public:
	GLuint programID;

	GLWindow();
	~GLWindow();

	void createVBO(GLuint size, Vertex* vertexData); //Vertex Buffer Object
	void createEBO(GLuint, GLuint* indexData); //Element Buffer Object
	void createShaders();
	void createVAO();

	void setVertexAttribPtr(GLuint attribLayoutLoc, GLint attribSize, GLint stride, int offset, GLenum dataType=GL_FLOAT, GLenum normalized=GL_FALSE);
	GLuint compileShader(GLuint shaderID, std::string shaderPath);
	void compileShaders();
	void creatProgram();

	static void handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void handleMouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void handleMouseBtnCallback(GLFWwindow* window, int button, int action, int mods);

	void getPollingUpdate();

	mat4 sendFullMatrix(int width, int height);
};