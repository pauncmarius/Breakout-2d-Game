
//rendererObject.cpp
#include "rendererObject.h"

const float ObjectRenderer::Vertices[] = {
    // Left Half
    0.0f, 1.0f, 0.0f, 1.0f, // bottom left
    0.5f, 1.0f, 0.5f, 1.0f, // center bottom
    0.0f, 0.0f, 0.0f, 0.0f, // top left

    0.5f, 0.0f, 0.5f, 0.0f, // center top
    0.5f, 1.0f, 0.5f, 1.0f, // center bottom
    0.0f, 0.0f, 0.0f, 0.0f, // top left

    // Right Half
    1.0f, 1.0f, 1.0f, 1.0f, // bottom right
    0.5f, 1.0f, 0.5f, 1.0f, // center bottom
    1.0f, 0.0f, 1.0f, 0.0f, // top right

    0.5f, 0.0f, 0.5f, 0.0f, // center top
    1.0f, 0.0f, 1.0f, 0.0f, // top right
    0.5f, 1.0f, 0.5f, 1.0f, // center bottom
};

ObjectRenderer::ObjectRenderer(Shader& shader, unsigned int width, unsigned int height) : 
    shader(shader), Width(width), Height(height), VAO(), VBO(nullptr)
{
    VBO = new VertexBuffer(Vertices, sizeof(Vertices));

    VAO.Bind();
    VBO->Bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    VBO->Unbind();
    VAO.Unbind();
}

ObjectRenderer::~ObjectRenderer()
{
    delete VBO;

}

void ObjectRenderer::DrawObject(Texture2D& texture, vec2 position, vec2 size, vec3 color, float alpha)
{
    // prepare transformations
    this->shader.Use();

    glUniform1i(glGetUniformLocation(this->shader.ID, "image"), 0);

    this->projection = ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f);
    glUniformMatrix4fv(glGetUniformLocation(this->shader.ID, "projection"), 1, false, value_ptr(this->projection));

    // Set alpha uniform
    glUniform1f(glGetUniformLocation(this->shader.ID, "alpha"), alpha);
    // model
    mat4 model = mat4(1.0f);
    model = translate(model, vec3(position, 0.0f)); 
    model = scale(model, vec3(size, 1.0f)); // last scale
    
    glUniformMatrix4fv(glGetUniformLocation(this->shader.ID, "model"), 1, false, value_ptr(model));
    // render textured quad
    glUniform3f(glGetUniformLocation(this->shader.ID, "spriteColor"), color.x, color.y, color.z);
    //texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
    //
    VAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 12);
    VAO.Unbind();
}