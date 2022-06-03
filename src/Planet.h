#pragma once

#include "e_type.h"
#include "Mesh.h"

#include <array>
#include <vector>

struct Coord final {
    f32 longitude{ 0.0f };
    f32 latitude{ 0.0f };

    static inline Coord fromPoint(v3 point) {
        return { std::asinf(point.y), std::atan2f(point.x, -point.z) };
    }
    static inline v3 toPoint(Coord coord) {
        const f32 r = std::cos(coord.latitude);
        return { std::sinf(coord.longitude) * r, std::sinf(coord.latitude), -std::cosf(coord.longitude) * r };
    }

    inline v3 toPoint() const {
        return toPoint(*this);
    }
};

struct Planet final {
    struct Face final {
        std::vector<v3> positions_;
        std::vector<u32> indices_;
        u32 indexCount_{ 0 };
        u32 resolution_{ 1 };
        v3 localUp_;
        v3 axisA_;
        v3 axisB_;
        VertexArrayObject vao_;
        Buffer<v3> vbo_;
        Buffer<u32> ibo_;

        inline void create(v3 localUp, int resolution = 1) {
            resolution_ = std::max(resolution, 1);
            localUp_ = localUp;
            axisA_ = v3{ localUp.y, localUp.z, localUp.x };
            axisB_ = glm::cross(localUp, axisA_);

            positions_.resize((resolution_ + 1) * (resolution_ + 1));
            indices_.resize(resolution_ * resolution_ * 6);
            indexCount_ = indices_.size();

            u32 triIndex{ 0 };

            for (int y{ 0 }; y <= resolution_; ++y) {
                for (int x{ 0 }; x <= resolution_; ++x) {
                    const auto i = x + y * (resolution_ + 1);
                    const v2 percent{ v2{ x, y } / static_cast<f32>(resolution_) };
                    const v3 pointOnUnitCube{ localUp_ + (percent.x - 0.5f) * 2.0f * axisA_ + (percent.y - 0.5f) * 2.0f * axisB_ };
                    positions_[i] = pointOnUnitCube;

                    if (x != resolution_ && y != resolution_) {
                        indices_[triIndex++] = i;
                        indices_[triIndex++] = i + 1;
                        indices_[triIndex++] = i + resolution_ + 2;
                        indices_[triIndex++] = i + resolution_ + 1;
                    }
                }
            }

            vbo_.create(positions_);
            ibo_.create(indices_);
            vao_.create();
            vao_.setVertexBuffer(vbo_, 0);
            vao_.setIndexBuffer(ibo_);
            vao_.setF32Attribute(0, 0, GL_FLOAT, 3, 0);


        }

        inline void draw() const {
            vao_.bind();
            glDrawElements(GL_PATCHES, indexCount_, GL_UNSIGNED_INT, nullptr);
            vao_.unbind();
        }
    };

    inline void init(u32 resolution = 10) {
        resolution_ = resolution;
        const std::array<v3, 6> directions = {
            v3{ 0.0f, 1.0f, 0.0f },
            v3{ 0.0f, -1.0f, 0.0f },
            v3{ 1.0f, 0.0f, 0.0f },
            v3{ -1.0f, 0.0f, 0.0f },
            v3{ 0.0f, 0.0f, 1.0f },
            v3{ 0.0f, 0.0f, -1.0f },
        };

        for (u32 i{ 0 }; i < 6; ++i) {
            terrainFaces_[i].create(directions[i], resolution_);
        }

    }

    inline void draw() const {
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        for (const auto& face: terrainFaces_) {
            face.draw();
        }
    }


    u32 resolution_{ 2 };
    std::array<Face, 6> terrainFaces_;
};