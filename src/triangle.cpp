#include "triangle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const char* vertexShaderSource = 
"#version 300 es\n"
"in vec2 aPos;\n"
"uniform mat4 uTransform;\n"
"void main() {\n"
"    gl_Position = uTransform * vec4(aPos, 0.0, 1.0);\n"
"}\n";

const char* fragmentShaderSource = 
"#version 300 es\n"
"precision mediump float;\n"
"out vec4 FragColor;\n"
"void main() {\n"
"    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
"}\n";

Triangle::Triangle() {
    shader = new Shader(vertexShaderSource, fragmentShaderSource);
    transformUniform = shader->getUniformLocation("uTransform");
    setupGeometry();
    std::cout << "Triangle initialized!" << std::endl;
}

Triangle::~Triangle() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete shader;
}

void Triangle::setupGeometry() {
    float vertices[] = {
        -0.5f, -0.5f,  // Bottom left
         0.5f, -0.5f,  // Bottom right
         0.0f,  0.5f   // Top center
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Triangle::render(float time) {
    shader->use();
    
    // Create rotation matrix using GLM
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(0.8f, 0.8f, 1.0f));
    
    glUniformMatrix4fv(transformUniform, 1, GL_FALSE, glm::value_ptr(transform));
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}