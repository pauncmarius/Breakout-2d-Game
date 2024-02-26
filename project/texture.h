
//texture.h
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stdexcept>

using namespace std;

class Texture2D
{
public:
    //Texture2D()
    Texture2D(){
        glGenTextures(1, &textureID);
        if (textureID == 0) {
            throw runtime_error("OpenGL failed to generate a texture ID.");
        }
    };

    //void Initialize(GLint width, GLint height, GLubyte* data)
    void Initialize(GLint width, GLint height, GLubyte* data) 
    {

        glBindTexture(GL_TEXTURE_2D, textureID);

        if (glGetError() != GL_NO_ERROR) {
            throw runtime_error::runtime_error("Failed to bind texture");
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    };

    // Getters for texture properties
    GLuint getTextureID() const { return textureID; }
private:
    GLuint textureID;
};
#endif
