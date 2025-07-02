#pragma once

#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include "shader.h"

class Triangle {
public:
    Triangle();
    ~Triangle();
    
    void render(float time);
    
private:
    GLuint VAO, VBO;
    Shader* shader;
    GLint transformUniform;
    
    void setupGeometry();
};