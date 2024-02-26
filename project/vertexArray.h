
//vertexArray.h
#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <glad/glad.h>

class VertexArray {
public:
    VertexArray() {
        glGenVertexArrays(1, &ID);
    }

    ~VertexArray() {
        glDeleteVertexArrays(1, &ID);
    }

    void Bind() const {
        glBindVertexArray(ID);
    }

    void Unbind() const {
        glBindVertexArray(0);
    }

private:
    unsigned int ID;
};

#endif
