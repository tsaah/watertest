#pragma once

class UniformBuffer final {
public:
    ~UniformBuffer() {
        if (id_ != 0) {
            glDeleteBuffers(1, &id_);
            id_ = 0;
        }
    }

    inline void create(GLuint binding) {
        if (id_ != 0) { return; }
        binding_ = binding;
        glGenBuffers(1, &id_);
    }

    template <class Interface>
    inline void create(GLuint binding, const Interface& data) {
        create(binding);
        setData(data);
    }

    template <class Interface>
    inline void setData(const Interface& data) {
        bind();
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Interface), &data, GL_STATIC_DRAW);
        unbind();
    }

    inline void bind() const {
        // glUniformBlockBinding(program_, idx, binding);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_, id_);
    }

    inline void unbind() const {
        // glUniformBlockBinding(program_, idx, 0);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_, 0);
    }

    operator GLuint() const { return id_; }

    GLuint id_{ 0 };
    GLuint binding_{ 0 };
};