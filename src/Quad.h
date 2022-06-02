#pragma once

#include "e_type.h"
#include "Buffer.h"
#include "VertexArrayObject.h"

#include <vector>

struct Quad final {
    struct Vertex final {
        v3 position;
        v3 normal;
        v2 uv;
    };

    inline void create() {
        const std::vector<Vertex> data = {
            { {  1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
            { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
            { {  1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
            { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        };
        vbo_.create(data);

        vao_.create();
        vao_.setVertexBuffer(vbo_, 0);
        vao_.setF32Attribute(0, 0, GL_FLOAT, 3, offsetof(Vertex, position));
        vao_.setF32Attribute(0, 1, GL_FLOAT, 3, offsetof(Vertex, normal));
        vao_.setF32Attribute(0, 4, GL_FLOAT, 2, offsetof(Vertex, uv));
    }

    inline void draw() const {
        vao_.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        vao_.unbind();
    }

    VertexArrayObject vao_;
    Buffer<Vertex> vbo_;
};