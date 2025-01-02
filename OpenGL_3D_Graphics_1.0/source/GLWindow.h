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

using std::vector;
using std::string;

enum updateType {
	noUpdate = 0b00,
	keyUpdate = 0b01,
	mouseUpdate = 0b10,
	mouseBtnUpdate = 0b100
};

enum glUnifromType {
	UNI_1I = 0b00,
	UNI_3FV = 0b01,
	UNI_M4FV = 0b10,
};

class GLWindow{

	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	std::vector<GLuint> vertexBufferIDs;
	std::vector<GLuint> indexBufferIDs;
	std::vector<GLuint> textureBufferIDs;

	std::vector<GLuint> vertexArrayIDs;

	static glm::vec3 pollKeyMovement;
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
	void createEBO(GLuint size, GLuint* indexData); //Element Buffer Object
	void createTexO(int width, int height, GLenum colorType, unsigned char* data);
	void createShaders();
	GLuint createVAO();
	void bindVAO(int index);
	void unbindVAO(int index);

	void setVertexAttribPtr(GLuint attribLayoutLoc, GLint attribSize, GLint stride, long offset, GLenum dataType=GL_FLOAT, GLenum normalized=GL_FALSE);
	void setMultipleAttribPtr(vector<glm::ivec3> attribList);

	GLuint compileShader(GLuint shaderID, std::string shaderPath);
	void compileShaders(std::string vshaderPath="VertexShaderCode.glsl", std::string fshaderPath="FragmentShaderCode.glsl");
	void creatProgram();

	static void handleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void handleMouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void handleMouseBtnCallback(GLFWwindow* window, int button, int action, int mods);

	void getPollingUpdate();

	mat4 generateMovementMat(vec3 position, vec3 rotation);
	void sendUniformComponents(int width, int height, glm::mat4 = mat4(1.0f), GLfloat FOV = 60.0f);

	template <typename T>
	void sendUniformData(int dataType, const char* dataName, T& data);

	void drawShape(ShapeBuilder& srcShape, int drawMethod, GLenum drawType = GL_TRIANGLES);
	void cleanUP();
};