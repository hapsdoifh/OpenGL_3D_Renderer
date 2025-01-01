// source.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <GLWindow.h>
#include <ShapeBuilder.h>
#include <glfw3.h>
#include <stb_image.h>

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

    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);

    GLWindow glwindow;


    glwindow.createShaders();
    glwindow.compileShaders();

    glwindow.creatProgram();

    //Cube1
    ShapeBuilder myCube1;
    myCube1.buildCube(1.0f, vec3(0.0,1.0f,1.0f));

    myCube1.vaoIndex = glwindow.createVAO();
    glwindow.createVBO(myCube1.vertexByteSize, myCube1.vertexData);
    glwindow.createEBO(myCube1.indexByteSize, myCube1.indexData);
    vector<glm::ivec3> attribList = {glm::ivec3(0,3,0), glm::ivec3(1,3,3), glm::ivec3(2,3,6)};
    glwindow.setMultipleAttribPtr(attribList);
    glwindow.unbindVAO(0);

    //Normals
    ShapeBuilder myNorms1;
    myNorms1.buildNormals(myCube1);

    myNorms1.vaoIndex = glwindow.createVAO();
    glwindow.createVBO(myNorms1.vertexByteSize, myNorms1.vertexData);
    glwindow.createEBO(myNorms1.indexByteSize, myNorms1.indexData);
    attribList = vector<glm::ivec3>{glm::ivec3(0,3,0), glm::ivec3(1,3,3), glm::ivec3(2,3,6)};
    glwindow.setMultipleAttribPtr(attribList);
    glwindow.unbindVAO(1);

    //ImportShape
    ShapeBuilder myImport1;
    myImport1.importShape("180212_Erik_XIV_Rustning_2.obj");

    myImport1.vaoIndex = glwindow.createVAO();
    glwindow.createVBO(myImport1.vertexByteSize, myImport1.vertexData);
    glwindow.createEBO(myImport1.indexByteSize, myImport1.indexData);
    int width, height, nChannel;
    unsigned char* texData = stbi_load("180212_Erik_XIV_Rustning_2_u1_v1.png", &width, &height, &nChannel, 0);

    GLenum colorType = nChannel == 3 ? GL_RGB : GL_RGBA;
    glwindow.createTexO(width, height, colorType, texData);
    stbi_image_free(texData);
    attribList = vector<glm::ivec3>{glm::ivec3(0,3,0), glm::ivec3(1,3,3), glm::ivec3(2,3,6), glm::ivec3(3,2,9)};
    glwindow.setMultipleAttribPtr(attribList);
    glwindow.unbindVAO(2);

    //Normals
    ShapeBuilder myNorms2;
    myNorms2.buildNormals(myImport1);

    glwindow.createVAO();
    glwindow.createVBO(myNorms2.vertexByteSize, myNorms2.vertexData);
    glwindow.createEBO(myNorms2.indexByteSize, myNorms2.indexData);
    glwindow.setVertexAttribPtr(0,3,sizeof(Vertex), 0);
    glwindow.unbindVAO(3);

    //Light Source
    ShapeBuilder myLight1;
    myLight1.buildCube(1.0f);

    myLight1.vaoIndex = glwindow.createVAO();
    glwindow.createVBO(myLight1.vertexByteSize, myLight1.vertexData);
    glwindow.createEBO(myLight1.indexByteSize, myLight1.indexData);
    attribList = vector<glm::ivec3>{glm::ivec3(0,3,0), glm::ivec3(1,3,3), glm::ivec3(2,3,6)};
    glwindow.setMultipleAttribPtr(attribList);

    glwindow.createShaders();
    glwindow.compileShaders();

    glwindow.creatProgram();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    while(!glfwWindowShouldClose(myWindow)) {
        int width, height;
        //what does this do? pulled it form glfw example page
        glfwGetFramebufferSize(myWindow, &width, &height);

        glViewport(0,0, width, height);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glCullFace(GL_BACK);
        glwindow.getPollingUpdate();

        glwindow.bindVAO(myCube1.vaoIndex);
        mat4 modWorldMat = glwindow.generateMovementMat(vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f,0.0f,30.0f));
        glwindow.sendUniformComponents(width, height, modWorldMat,80.0f);
        glwindow.drawShape(myCube1, 0);

        glwindow.bindVAO(myNorms1.vaoIndex);
        glwindow.drawShape(myNorms1, 1, GL_LINES);

        glwindow.bindVAO(myImport1.vaoIndex);
        // GLint texLoc = glGetUniformLocation(glwindow.programID, "texture0");
        // glUniform1i(texLoc, 0);
        glwindow.sendUniformData(glwindow.programID, "texture0", 0);
        modWorldMat = glwindow.generateMovementMat(vec3(0.0f, -10.0f, -10.0f), glm::vec3(-90.0f,180.0f,0.0f));
        glwindow.sendUniformComponents(width, height, modWorldMat,80.0f);
        glwindow.drawShape(myImport1, 0);

        modWorldMat = glwindow.generateMovementMat(vec3(0.0f, -10.0f, 10.0f), glm::vec3(-90.0f,0.0f,0.0f));
        glwindow.sendUniformComponents(width, height, modWorldMat,80.0f);
        glwindow.drawShape(myImport1, 0);

        // glwindow.bindVAO(3);
        // glDrawArrays(GL_LINES, 0, myNorms2.numVertices);

        glwindow.bindVAO(myLight1.vaoIndex);
        modWorldMat = glwindow.generateMovementMat(vec3(0.0f, 50.0f, 0.0f), glm::vec3(0.0f,0.0f,0.0f));
        glwindow.sendUniformComponents(width, height, modWorldMat,80.0f);
        glwindow.drawShape(myLight1, 0);

        glfwSwapBuffers(myWindow);

        //absolutely necessary otherwise you get unclosable transparent window that hogs resources
        glfwPollEvents();
    }

    myCube1.cleanUP();
    myNorms1.cleanUP();
    myImport1.cleanUP();
    myNorms2.cleanUP();

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

