// source.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <GLWindow.h>
#include <ShapeBuilder.h>
#include <glfw3.h>

int main(int argc, char* argv[])
{
    if(!glfwInit()) {
        std::cout << "glfw initialization failed, exitting";
        exit(0);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* myWindow = glfwCreateWindow(640, 480, "OpenGL_Window", NULL, NULL);
    glfwMakeContextCurrent(myWindow);
    glfwSwapInterval(1);

    glfwSetKeyCallback(myWindow, &GLWindow::handleKeyCallback);
    glfwSetCursorPosCallback(myWindow, &GLWindow::handleMouseCallback);
    glfwSetMouseButtonCallback(myWindow, &GLWindow::handleMouseBtnCallback);
    // std::cout << gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // if(!myWindow) {
    //     glfwTerminate();
    //     exit(0);
    // }
    // GLenum err = glGetError();
    // if (err != GL_NO_ERROR) {
    //     std::cerr << "OpenGL error: " << err << std::endl;
    // }

    gladLoadGL(); //doesn't work without this line

    // const GLubyte* version = glGetString(GL_VERSION);
    // const GLubyte* renderer = glGetString(GL_RENDERER);
    // const GLubyte* vendor = glGetString(GL_VENDOR);

    GLWindow glwindow;
    ShapeBuilder myCube1;
    myCube1.buildCube(1.0f);

    ShapeBuilder myNorms1;
    myNorms1.buildNormals(myCube1);

    ShapeBuilder myImport1;
    myImport1.importShape("Datsun_280Z.obj");

    ShapeBuilder myNorms2;
    myNorms2.buildNormals(myImport1);

    glwindow.createShaders();
    glwindow.compileShaders();

    glwindow.creatProgram();

    //Cube1
    glwindow.createVAO();
    glwindow.createVBO(myCube1.vertexByteSize, myCube1.vertexData);
    glwindow.createEBO(myCube1.indexByteSize, myCube1.indexData);
    glwindow.setVertexAttribPtr(0,3,9 * sizeof(GLfloat), 0);
    glwindow.setVertexAttribPtr(1,3,9 * sizeof(GLfloat), 3 * sizeof(GLfloat));
    glwindow.setVertexAttribPtr(2,3,9 * sizeof(GLfloat), 6 * sizeof(GLfloat));
    glwindow.unbindVAO(0);

    //Normals
    glwindow.createVAO();
    glwindow.createVBO(myNorms1.vertexByteSize, myNorms1.vertexData);
    glwindow.createEBO(myNorms1.indexByteSize, myNorms1.indexData);
    glwindow.setVertexAttribPtr(0,3,9 * sizeof(GLfloat), 0);
    glwindow.unbindVAO(1);

    //ImportCube
    glwindow.createVAO();
    glwindow.createVBO(myImport1.vertexByteSize, myImport1.vertexData);
    glwindow.createEBO(myImport1.indexByteSize, myImport1.indexData);
    glwindow.setVertexAttribPtr(0,3,9*sizeof(GLfloat), 0);
    glwindow.setVertexAttribPtr(1,3,9*sizeof(GLfloat), 3 * sizeof(GLfloat));
    glwindow.setVertexAttribPtr(2,3,9*sizeof(GLfloat), 6 * sizeof(GLfloat));
    glwindow.unbindVAO(2);


    //Normals
    glwindow.createVAO();
    glwindow.createVBO(myNorms2.vertexByteSize, myNorms2.vertexData);
    glwindow.createEBO(myNorms2.indexByteSize, myNorms2.indexData);
    glwindow.setVertexAttribPtr(0,3,9 * sizeof(GLfloat), 0);
    glwindow.unbindVAO(3);


    glwindow.createShaders();
    glwindow.compileShaders();

    glwindow.creatProgram();
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);

    while(!glfwWindowShouldClose(myWindow)) {
        int width, height;
        //what does this do? pulled it form glfw example page
        glfwGetFramebufferSize(myWindow, &width, &height);

        glViewport(0,0, width, height);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

        // glCullFace(GL_BACK);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glwindow.getPollingUpdate();

        mat4 modWorldMat = glwindow.generateMovementMat(vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f,0.0f,30.0f));
        glwindow.sendUniformComponents(width, height, modWorldMat);
        glwindow.bindVAO(0);
        glDrawElements(GL_TRIANGLES, myCube1.numIndices, GL_UNSIGNED_INT, (void*)0);

        glwindow.bindVAO(1);
        glDrawArrays(GL_LINES, 0, myNorms1.numVertices);

        modWorldMat = glwindow.generateMovementMat(vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f,20.0f,0.0f));
        glwindow.sendUniformComponents(width, height, modWorldMat);
        glwindow.bindVAO(2);
        // glDrawElements(GL_TRIANGLES, myImport1.numIndices, GL_UNSIGNED_INT, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, myImport1.numVertices);

        glwindow.bindVAO(3);
        // glDrawArrays(GL_LINES, 0, myNorms2.numVertices);


        glfwSwapBuffers(myWindow);
        //absolutely necessary otherwise you get unclosable transparent window that hogs resources
        glfwPollEvents();
    }
    //
    myCube1.cleanUP();
    myNorms1.cleanUP();
    myImport1.cleanUP();

    glfwDestroyWindow(myWindow);
    glfwTerminate();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug prograam: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

