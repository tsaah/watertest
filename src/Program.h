#pragma once

#include "Shader.h"

#include <unordered_map>

class Program final {
public:
    ~Program() {
        if (id_ != 0) {
            glDeleteProgram(id_);
            id_ = 0;
        }
    }

    inline void create(const std::vector<Shader>& shaders) {
        if (id_ != 0) { return; }

        id_ = glCreateProgram();

        for (const auto& shader: shaders) {
            glAttachShader(id_, shader);
        }
        glLinkProgram(id_);

        GLint programLinked = GL_FALSE;
        glGetProgramiv(id_, GL_LINK_STATUS, &programLinked);
        if (!programLinked) {
            std::cerr << "Shader linking failed:" << std::endl;
            char infoLog[512];
            glGetShaderInfoLog(id_, 512, nullptr, infoLog);
            std::cerr << infoLog << std::endl;
            return;
        }

        for (const auto& shader: shaders) {
            glDetachShader(id_, shader);
        }
    }

    inline void bind() const { glUseProgram(id_); }

    inline void unbind() const { glUseProgram(0); };

    operator GLuint() const { return id_; }

    GLuint id_{ 0 };
};