// ShaderManager.h

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <map>
#include <string>
#include "shader.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
using namespace glm;

class ShaderManager {
public:
    static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, string name);
    static Shader GetShader(string name);
    static void Clear();

private:
    static map<string, Shader> Shaders;
    ShaderManager() {}

    static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile);
    static string readShaderCode(const char* shaderFile);
};

#endif
