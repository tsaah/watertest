#pragma once

#include "e_type.h"

#include <glad/glad.h>
#include <stb_image.h>

struct Texture2D final {
    inline void load(const std::string& filename) {
        int w;
        int h;
        int comp;
        // u8* image = stbi_load(filename.c_str(), &w, &h, &comp, STBI_default);
        // u8* image = stbi_load("textures/BricksPaintedWhite001_DISP_4K.jpg", &w, &h, &comp, STBI_default);
        stbi_set_flip_vertically_on_load(true);

        // u8* image = stbi_load("textures/HuffmanPatterson_Figure1.png", &w, &h, &comp, STBI_default);
        u16* image = stbi_load_16("textures/gebco_21600x10800_16bit_0_0.png", &w, &h, &comp, STBI_default);
        // image = stbi_load_16("textures/gebco_4096x2048_16bit_0_0.png", &w, &h, &comp, STBI_default);
        // u8* image = stbi_load("textures/gebco_4096x2048_8bit_0_0.png", &w, &h, &comp, STBI_default);

        if(image == nullptr)
            return;

        glCreateTextures(GL_TEXTURE_2D, 1, &id_);

        glTextureParameteri(id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(id_, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glTextureStorage2D(id_, 1, GL_R16, w, h);
        glTextureSubImage2D(id_, 0, 0, 0, w, h, GL_RED, GL_UNSIGNED_SHORT, image);
        // glTextureStorage2D(id_, 1, GL_RGBA8, w, h);
        // glTextureSubImage2D(id_, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateTextureMipmap(id_);

        stbi_image_free(image);
    }
    ~Texture2D() {
        if (id_ != 0) {
            glDeleteTextures(1, &id_);
            id_ = 0;
        }
    }

    inline void bind(GLuint unit) const {
        glBindTextureUnit(unit, id_);
    }

    inline void unbind(GLuint unit) const {
        glBindTextureUnit(unit, 0);
    }

    GLuint id_{ 0 };
};