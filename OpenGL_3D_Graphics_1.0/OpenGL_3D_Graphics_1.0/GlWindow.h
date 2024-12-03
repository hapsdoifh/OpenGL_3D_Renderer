
#ifndef GL_WINDOW
#define GL_WINDOW
#include <QtOpenGLWidgets/qopenglwidget.h>

class GlWindow : public QOpenGLWidget
{
public:
	~GlWindow();
protected:
	void initializeGL() override;
	void paintGL() override;

private:
	void sendDataToOpenGL();
	void installShaders();
	bool checkShaderStatus(GLuint ShaderID);
	bool checkProgramStatus(GLuint programID);
	std::string readShaderFile(const char* fileName);
	void sendTriOpenGL();

};

#endif
