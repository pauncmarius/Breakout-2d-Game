
//renderText.cpp
#include "rendererText.h"
#include <iostream>
#include "shaderManager.h"

TextRenderer::TextRenderer(Shader& shader, unsigned int width, unsigned int height)
    : atlasWidth(1024), atlasHeight(1024), TextShader(shader), Width(width), Height(height) {
    // Configure VAO/VBO for texture quads
    float vertices[6 * 4]; // Adjust size according to your needs
    VBO = new VertexBuffer(vertices, sizeof(vertices));

    VAO.Bind();
    VBO->Bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    VBO->Unbind();
    VAO.Unbind();
}

TextRenderer::~TextRenderer() {
    delete VBO; // Clean up dynamically allocated VBO
    glDeleteTextures(1, &this->textureAtlas);
}

void TextRenderer::Load(const string& font, unsigned int fontSize) {
    this->Characters.clear();
    createTextureAtlas(font, fontSize);
}

void TextRenderer::RenderText(string text, float x, float y, float scale, vec3 color) {
    this->TextShader.Use();
    glUniform3f(glGetUniformLocation(this->TextShader.ID, "textColor"), color.x, color.y, color.z);
    mat4 projection = ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f);
    glUniformMatrix4fv(glGetUniformLocation(this->TextShader.ID, "projection"), 1, GL_FALSE, value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureAtlas);
    VAO.Bind();

    // Iterate through all characters
    string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - ch.Bearing.y * scale;


        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // Update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   ch.uvTopLeft.x,    ch.uvBottomRight.y },
            { xpos,     ypos,       ch.uvTopLeft.x,    ch.uvTopLeft.y },
            { xpos + w, ypos,       ch.uvBottomRight.x, ch.uvTopLeft.y },

            { xpos,     ypos + h,   ch.uvTopLeft.x,    ch.uvBottomRight.y },
            { xpos + w, ypos,       ch.uvBottomRight.x, ch.uvTopLeft.y },
            { xpos + w, ypos + h,   ch.uvBottomRight.x, ch.uvBottomRight.y }
        };
        // Render glyph texture over quad
        VBO->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph
        x += (ch.Advance >> 6) * scale;
    }
    VAO.Unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::createTextureAtlas(const string& font, unsigned int fontSize) {
    // Initialize FreeType
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
        return;
    }

    // Load font
    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face)) {
        cout << "ERROR::FREETYPE: Failed to load font" << endl;
        return;
    }

    // Set font size
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Create texture for the atlas
    glGenTextures(1, &this->textureAtlas);
    glBindTexture(GL_TEXTURE_2D, this->textureAtlas);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int xOffset = 0, yOffset = 0;
    unsigned int rowHeight = 0;

    // Load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++) {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            cout << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
            continue;
        }

        // Check if we need to advance to next row
        if (xOffset + face->glyph->bitmap.width > atlasWidth) {
            xOffset = 0;
            yOffset += rowHeight;
            rowHeight = 0;
        }

        // Generate texture for the character
        glBindTexture(GL_TEXTURE_2D, this->textureAtlas);
        glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // Calculate UV coordinates
        vec2 uvTopLeft = vec2((float)xOffset / atlasWidth, (float)yOffset / atlasHeight);
        vec2 uvBottomRight = vec2((float)(xOffset + face->glyph->bitmap.width) / atlasWidth, (float)(yOffset + face->glyph->bitmap.rows) / atlasHeight);

        // Store character information
        Character character = {
            uvTopLeft,
            uvBottomRight,
            ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(pair<char, Character>(c, character));

        // Advance xOffset for next glyph and update rowHeight
        xOffset += face->glyph->bitmap.width;
        rowHeight = std::max(rowHeight, face->glyph->bitmap.rows);
    }

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // Clean up FreeType resources
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

float TextRenderer::CalculateTextWidth(const string& text, float scale) {
    float width = 0.0f;
    for (char c : text) {
        Character ch = Characters[c];
        float characterWidth = (ch.Advance >> 6) * scale; // Convert advance to pixels and scale
        width += characterWidth;
    }
    return width;
}

