#pragma once

#include <GLES3/gl3.h>

class Shader {
public:
    GLuint programId;
    
    Shader(const char* vertexSource, const char* fragmentSource);
    ~Shader();
    
    void use();
    GLint getUniformLocation(const char* name);
    
private:
    GLuint compileShader(GLenum type, const char* source);
};