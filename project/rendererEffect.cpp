
//rendererEffect.cpp
#include "rendererEffect.h"

const float EffectRenderer::vertices[] = {
    // First Half (Left)
    -1.0f, -1.0f, 0.0f, 0.0f, // bottom left corner
     0.0f, -1.0f, 0.5f, 0.0f, // bottom center
    -1.0f,  1.0f, 0.0f, 1.0f, // top left corner

     0.0f, -1.0f, 0.5f, 0.0f, // bottom center
     0.0f,  1.0f, 0.5f, 1.0f, // top center
    -1.0f,  1.0f, 0.0f, 1.0f, // top left corner

    // Second Half (Right)
     0.0f, -1.0f, 0.5f, 0.0f, // bottom center
     1.0f, -1.0f, 1.0f, 0.0f, // bottom right corner
     0.0f,  1.0f, 0.5f, 1.0f, // top center

     1.0f, -1.0f, 1.0f, 0.0f, // bottom right corner
     1.0f,  1.0f, 1.0f, 1.0f, // top right corner
     0.0f,  1.0f, 0.5f, 1.0f  // top center
};


EffectRenderer::EffectRenderer(Shader& shader, unsigned int width, unsigned int height, float blur)
    : effectProcessingShader(shader), Width(width), Height(height), Confuse(false), BlurAmount(blur), VAO(), VBO(nullptr)
{
    initFramebuffers();

    VBO = new VertexBuffer(vertices, sizeof(vertices));

    VAO.Bind();
    VBO->Bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    VBO->Unbind();
    VAO.Unbind();
}

EffectRenderer::~EffectRenderer() {
    delete VBO; // Clean up the dynamically allocated VBO
    glDeleteFramebuffers(1, &this->FBO);
}


void EffectRenderer::BeginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void EffectRenderer::EndRender()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Render the post-processed image
void EffectRenderer::Render(float time)
{
    this->effectProcessingShader.Use();

    mat4 projection = ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f);
    glUniformMatrix4fv(glGetUniformLocation(this->effectProcessingShader.ID, "projection"), 1, false, value_ptr(projection));
    glUniform1f(glGetUniformLocation(this->effectProcessingShader.ID, "time"), time);
    glUniform1i(glGetUniformLocation(this->effectProcessingShader.ID, "confuse"), this->Confuse);
    glUniform1f(glGetUniformLocation(this->effectProcessingShader.ID, "blurAmount"), this->BlurAmount);
    glUniform1i(glGetUniformLocation(this->effectProcessingShader.ID, "scene"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->Texture.getTextureID());
    VAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 12);
    VAO.Unbind();
}

void EffectRenderer::initFramebuffers()
{
    // Framebuffer configuration
    glGenFramebuffers(1, &this->FBO);
    // Bind and configure the standard framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->Texture.Initialize(this->Width, this->Height, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.getTextureID(), 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::EffectRenderer: Failed to initialize FBO" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}