#pragma once

#include "e_type.h"
#include "Buffer.h"
#include "VertexArrayObject.h"

#include <vector>


class Mesh final {
public:
    struct Vertex final {
        v3 position;
        v3 normal;
        v3 color;
        v4 tangent;
        v2 uv;
    };
    std::vector<Vertex> vertices;
    std::vector<u32> indices;
    u32 indexCount_{ 0 };

    VertexArrayObject vao_;
    Buffer<Vertex> vbo_;
    Buffer<u32> ibo_;

    inline void create() {
        vbo_.create(vertices);
        ibo_.create(indices);
        indexCount_ = indices.size();

        vao_.create();
        vao_.setVertexBuffer(vbo_, 0);
        vao_.setIndexBuffer(ibo_);
        vao_.setF32Attribute(0, 0, GL_FLOAT, 3, offsetof(Vertex, position));
        vao_.setF32Attribute(0, 1, GL_FLOAT, 3, offsetof(Vertex, normal));
        vao_.setF32Attribute(0, 2, GL_FLOAT, 4, offsetof(Vertex, tangent));
        vao_.setF32Attribute(0, 3, GL_FLOAT, 3, offsetof(Vertex, color));
        vao_.setF32Attribute(0, 4, GL_FLOAT, 2, offsetof(Vertex, uv));
    }

    inline void draw() const {
        vao_.bind();
        glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, nullptr);
        vao_.unbind();
    }
};

