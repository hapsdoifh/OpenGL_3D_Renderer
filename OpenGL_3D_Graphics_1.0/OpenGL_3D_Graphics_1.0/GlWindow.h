
#ifndef GL_WINDOW
#define GL_WINDOW
#include <QtOpenGLWidgets/qopenglwidget.h>
//#include <QtWidgets/qapplication.h>

class GlWindow : public QOpenGLWidget
{
public:
	~GlWindow();
protected:
	void initializeGL() override;
	void paintGL() override;
	void mouseMoveEvent(QMouseEvent* event);
	void keyPressEvent(QKeyEvent* event);

private:
	void sendDataToOpenGL();
	void installShaders();
	bool checkShaderStatus(GLuint ShaderID);
	bool checkProgramStatus(GLuint programID);
	std::string readShaderFile(const char* fileName);
	void sendTriOpenGL();
	void setupVertexArrays();
};

#endif
