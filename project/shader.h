
//shader.h
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

class Shader
{
public:
    unsigned int ID;

    Shader(){}

    // Use the shader program
    Shader& Use() {
        glUseProgram(this->ID);
        return *this;
    }

    // Compile the shader from the given vertex and fragment source code
    void Compile(const char* vertexSource, const char* fragmentSource) {
        unsigned int sVertex, sFragment;

        // Compile vertex shader
        sVertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(sVertex, 1, &vertexSource, NULL);
        glCompileShader(sVertex);

        // Compile fragment shader
        sFragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(sFragment, 1, &fragmentSource, NULL);
        glCompileShader(sFragment);

        // Create shader program
        this->ID = glCreateProgram();
        glAttachShader(this->ID, sVertex);
        glAttachShader(this->ID, sFragment);
        glLinkProgram(this->ID);

        // Delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(sVertex);
        glDeleteShader(sFragment);
    }
};

#endif