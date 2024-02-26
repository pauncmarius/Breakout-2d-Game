
// shaderManager.cpp
#include "shaderManager.h"

map<string, Shader> ShaderManager::Shaders;

Shader ShaderManager::LoadShader(const char* vShaderFile, const char* fShaderFile, string name) {
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
    return Shaders[name];
}

Shader ShaderManager::GetShader(string name) {
    return Shaders.at(name);
}

void ShaderManager::Clear() {
    for (auto& iter : Shaders) {
        glDeleteProgram(iter.second.ID);
    }
    Shaders.clear();
}

Shader ShaderManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile) {
    string vertexCode, fragmentCode;
    try {
        vertexCode = readShaderCode(vShaderFile);
        fragmentCode = readShaderCode(fShaderFile);
    }
    catch (const exception& e) {
        cerr << "ERROR::SHADER: Failed to read shader files: " << e.what() << endl;
    }

    Shader shader;
    shader.Compile(vertexCode.c_str(), fragmentCode.c_str());
    return shader;
}

string ShaderManager::readShaderCode(const char* shaderFile) {
    ifstream file(shaderFile);
    if (!file) {
        throw runtime_error("Failed to open shader file.");
    }
    stringstream shaderStream;
    shaderStream << file.rdbuf();
    file.close();
    return shaderStream.str();
}