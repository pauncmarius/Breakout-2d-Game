//rendererEffect.h
// *info* -> effect_render.cpp

#ifndef EFFECT_RENDERER_H
#define EFFECT_RENDERER_H

#include <glad/glad.h>
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <iostream>
#include "vertexArray.h"
#include "vertexBuffer.h"

using namespace std;
using namespace glm;

class EffectRenderer
{
public:
    Shader effectProcessingShader;
    Texture2D Texture;
    unsigned int Width, Height;
    bool Confuse;
    float BlurAmount;

    EffectRenderer(Shader& shader, unsigned int width, unsigned int height, float blur);
    ~EffectRenderer();

    void BeginRender();
    void EndRender();
    void Render(float time);

private:
    VertexArray         VAO;
    VertexBuffer*       VBO; // Pointer because we will allocate it dynamically
    unsigned int        FBO;
    static const float  vertices[];

    void initFramebuffers();
};

#endif