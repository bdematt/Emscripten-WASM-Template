#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "triangle.h"

// Global variables for the main loop
GLFWwindow* window;
Triangle* triangle;

void render() {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    float time = (float)glfwGetTime();
    triangle->render(time);
}

void main_loop() {
    if (glfwWindowShouldClose(window)) {
        emscripten_cancel_main_loop();
        return;
    }
    
    glfwPollEvents();
    render();
    glfwSwapBuffers(window);
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    std::cout << "GLFW initialized successfully!" << std::endl;
    
    // Configure GLFW for WebGL2/OpenGL ES 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    
    window = glfwCreateWindow(800, 600, "Multi-file WebGL Test", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    std::cout << "Window created successfully!" << std::endl;
    
    glfwMakeContextCurrent(window);
    
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    // Initialize our triangle
    triangle = new Triangle();
    
    emscripten_set_main_loop(main_loop, 0, 1);
    
    // Cleanup (won't be reached but good practice)
    delete triangle;
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}