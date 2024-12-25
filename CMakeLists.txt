cmake_minimum_required(VERSION 3.29)
project(OpenGL_3D_Renderer)

set(CMAKE_CXX_STANDARD 14)

include_directories(OpenGL_3D_Graphics_1.0/Dependencies/glad/include)
include_directories(OpenGL_3D_Graphics_1.0/Dependencies/glad/include/glad)
include_directories(OpenGL_3D_Graphics_1.0/Dependencies/glad/include/KHR)
include_directories(OpenGL_3D_Graphics_1.0/Dependencies/GLFW)
include_directories(OpenGL_3D_Graphics_1.0/Dependencies)
include_directories(OpenGL_3D_Graphics_1.0/source)

add_subdirectory(OpenGL_3D_Graphics_1.0/Dependencies/glm)

#add_library(libglfw STATIC OpenGL_3D_Graphics_1.0/Dependencies/GLFW/glfw3dll.lib)

add_executable(OpenGL_3D_Renderer
    OpenGL_3D_Graphics_1.0/Dependencies/glad/include/glad/glad.h
    OpenGL_3D_Graphics_1.0/Dependencies/glad/include/KHR/khrplatform.h
    OpenGL_3D_Graphics_1.0/Dependencies/glad/src/glad.c
    OpenGL_3D_Graphics_1.0/Dependencies/GLFW/glfw3.h
    OpenGL_3D_Graphics_1.0/Dependencies/GLFW/glfw3native.h
    OpenGL_3D_Graphics_1.0/source/GLWindow.h
    OpenGL_3D_Graphics_1.0/source/MainWindow.cpp
    OpenGL_3D_Graphics_1.0/source/stb_image.cpp
    OpenGL_3D_Graphics_1.0/source/stb_image.h
    OpenGL_3D_Graphics_1.0/source/GLWindow.cpp
    OpenGL_3D_Graphics_1.0/source/ShapeBuilder.cpp
    OpenGL_3D_Graphics_1.0/source/ShapeBuilder.h
    OpenGL_3D_Graphics_1.0/source/Camera.cpp
    OpenGL_3D_Graphics_1.0/source/Camera.h)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(OpenGL_3D_Renderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/OpenGL_3D_Graphics_1.0/Dependencies/GLFW/libglfw.3.dylib)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(OpenGL_3D_Renderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/OpenGL_3D_Graphics_1.0/Dependencies/GLFW/glfw3dll.lib)
endif()