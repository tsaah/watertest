#pragma once

#include <glad/glad.h>

#include <vector>

template <class Vertex>
class Buffer final {
public:
    ~Buffer() {
        if (id_ != 0) {
            glDeleteBuffers(1, &id_);
            id_ = 0;
        }
    }

    inline void create() {
        if (id_ != 0) { return; }
        glCreateBuffers(1, &id_);
    }

    inline void create(const std::vector<Vertex>& data) {
        create();
        setData(data);
    }

    inline void setData(const std::vector<Vertex>& data) {
        glNamedBufferStorage(id_, sizeof(Vertex) * data.size(), data.data(), GL_DYNAMIC_STORAGE_BIT);
    }

    operator GLuint() const { return id_; }

    GLuint id_{ 0 };
};