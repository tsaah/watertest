#pragma once

#include "FileReader.h"

// #define GLFW_INCLUDE_NONE
// #include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

class Shader final {
public:
    enum class Type: GLenum {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Gometry = GL_GEOMETRY_SHADER,
        TesselationContrl = GL_TESS_CONTROL_SHADER,
        TesselationEvaluation = GL_TESS_EVALUATION_SHADER,
        Compute = GL_COMPUTE_SHADER
    };

    ~Shader() {
        if (id_ != 0) {
            glDeleteShader(id_);
            id_ = 0;
        }
    }

    inline explicit Shader(const std::string& filename, Type shaderType) {
        create(filename, shaderType);
    }

    inline void create(const std::string& filename, Type shaderType) {
        if (id_ != 0) { return; }

        type_ = static_cast<GLenum>(shaderType);
        const auto shaderSource = FileReader::read(filename);
        if (shaderSource.empty()) {
            std::cout << "failed to load shader source" << filename << std::endl;
            return;
        }

        id_ = glCreateShader(type_);

        const auto* shaderSourcePtr = shaderSource.c_str();
        glShaderSource(id_, 1, &shaderSourcePtr, nullptr);
        glCompileShader(id_);

        GLint shaderCompiled = GL_FALSE;
        glGetShaderiv(id_, GL_COMPILE_STATUS, &shaderCompiled);
        if (shaderCompiled != GL_TRUE) {
            std::cout << "Unable to compile shader " << filename << " Source: " << shaderSource << std::endl;
            char infoLog[512];
            glGetShaderInfoLog(id_, 512, nullptr, infoLog);
            std::cout << infoLog << std::endl;
        }
    }

    operator GLuint() const { return id_; }

    GLenum type_{ GL_VERTEX_SHADER };
    GLuint id_{ 0 };
};