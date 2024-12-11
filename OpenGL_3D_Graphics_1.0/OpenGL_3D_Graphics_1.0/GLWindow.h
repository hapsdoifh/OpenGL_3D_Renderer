#pragma once
#include <QtOpenGLWidgets/qopenglwidget.h>
#include <glm/glm.hpp>

class GLWindow : public QOpenGLWidget{
	
	
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