// source.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <GLWindow.h>
#include <iostream>
#include <glfw3.h>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(int argc, char* argv[])
{
    if(!glfwInit()) {
        std::cout << "glfw initialization failed, exitting";
        exit(0);
    }
    GLFWwindow* myWindow = glfwCreateWindow(640, 480, "OpenGL_Window", NULL, NULL);
    glfwMakeContextCurrent(myWindow);
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
    glfwSetKeyCallback(myWindow, key_callback);
    while(!glfwWindowShouldClose(myWindow)) {
        int width, height;
        glfwGetFramebufferSize(myWindow, &width, &height);
        glViewport(0,0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
	    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

        glfwSwapBuffers(myWindow);

        //absolutely neccessary otherwise you get unclosable transparent window that hogs resources
        glfwPollEvents();
    }

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
