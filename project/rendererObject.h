// asset_renderer.h

#ifndef OBJECT_RENDERER_H
#define OBJECT_RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"
#include "shader.h"
#include <cstdlib>
#include <ctime>
#include "vertexArray.h"
#include "vertexBuffer.h"

using namespace std;
using namespace glm;

class ObjectRenderer
{
public:
    unsigned int Width, Height;

    ObjectRenderer(Shader& shader, unsigned int width, unsigned int height);
    ~ObjectRenderer();

    void DrawObject(Texture2D& texture, vec2 position, vec2 size, vec3 color = vec3(1.0f), float alpha = 1.0f);
private:
    Shader              shader;
    VertexArray         VAO;
    VertexBuffer*       VBO; // Pointer because we will allocate it dynamically
    mat4                projection;
    static const float  Vertices[];
};

#endif
