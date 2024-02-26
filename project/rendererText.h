#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"
#include <ft2build.h>
#include <algorithm>
#include "vertexArray.h"
#include "vertexBuffer.h"

#include FT_FREETYPE_H

using namespace std;
using namespace glm;

struct Character {
    vec2 uvTopLeft;      // UV coordinates of top-left corner of glyph in texture atlas
    vec2 uvBottomRight;  // UV coordinates of bottom-right corner of glyph
    ivec2 Size;          // Size of glyph
    ivec2 Bearing;       // Offset from baseline to left/top of glyph
    unsigned int Advance; // Horizontal offset to advance to next glyph
};

class TextRenderer {
public:
    unsigned int Width, Height;

    TextRenderer(Shader& shader, unsigned int width, unsigned int height);
    ~TextRenderer();

    void Load(const string& font, unsigned int fontSize);
    void RenderText(string text, float x, float y, float scale, vec3 color = vec3(1.0f));
    float CalculateTextWidth(const string& text, float scale);

private:
    unsigned int textureAtlas;
    map<char, Character> Characters;
    Shader TextShader;
    unsigned int atlasWidth, atlasHeight;
    static const float vertices;
    VertexArray VAO;
    VertexBuffer* VBO; // Pointer because we will allocate it dynamically

    void createTextureAtlas(const string& font, unsigned int fontSize);
};

#endif
