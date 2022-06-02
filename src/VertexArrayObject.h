#pragma once

#include "Buffer.h"

#include <glad/glad.h>

class VertexArrayObject final{
public:
    ~VertexArrayObject() {
        if (id_ != 0) {
            glDeleteProgram(id_);
            id_ = 0;
        }
    }

    inline void create() {
        if (id_ != 0) { return; }
        glCreateVertexArrays(1, &id_);
    }

    template <class Vertex>
    inline void setVertexBuffer(const Buffer<Vertex>& vbo, GLuint binding, GLuint startIndex = 0) {
        glVertexArrayVertexBuffer(id_, binding, vbo, startIndex, sizeof(Vertex));
    }

    inline void setF32Attribute(GLuint binding, GLuint location, GLenum type, GLuint size, GLuint relativeOffset) {
        glEnableVertexArrayAttrib(id_, location);
        glVertexArrayAttribFormat(id_, location, size, type, GL_FALSE, relativeOffset); // offsetof(glm::vec2, x)
        glVertexArrayAttribBinding(id_, location, binding);
    }

    template <class Index>
    inline void setIndexBuffer(const Buffer<Index>& ibo) {
        glVertexArrayElementBuffer(id_, ibo);
    }

    inline void bind() const { glBindVertexArray(id_); }

    inline void unbind() const { glBindVertexArray(0); };

    operator GLuint() const { return id_; }

    GLuint id_{ 0 };
};