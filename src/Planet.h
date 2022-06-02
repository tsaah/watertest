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
        Mesh mesh_;
        u32 resolution_{ 1 };
        v3 localUp_;
        v3 axisA_;
        v3 axisB_;

        inline void create(v3 localUp, int resolution = 1) {
            resolution_ = std::max(resolution, 1);
            localUp_ = localUp;
            axisA_ = v3{ localUp.y, localUp.z, localUp.x };
            axisB_ = glm::cross(localUp, axisA_);

            mesh_.vertices.resize((resolution_ + 1) * (resolution_ + 1));

            auto& indices = mesh_.indices;
            indices.resize(resolution_ * resolution_ * 6);

            u32 triIndex{ 0 };

            for (int y{ 0 }; y <= resolution_; ++y) {
                for (int x{ 0 }; x <= resolution_; ++x) {
                    const auto i = x + y * (resolution_ + 1);
                    auto& position = mesh_.vertices[i].position;
                    auto& normal = mesh_.vertices[i].normal;
                    const v2 percent{ v2{ x, y } / static_cast<f32>(resolution_) };
                    const v3 pointOnUnitCube{ localUp_ + (percent.x - 0.5f) * 2.0f * axisA_ + (percent.y - 0.5f) * 2.0f * axisB_ };
                    const v3 t{
                        pointOnUnitCube.x * pointOnUnitCube.x,
                        pointOnUnitCube.y * pointOnUnitCube.y,
                        pointOnUnitCube.z * pointOnUnitCube.z
                    };
                    const v3 pointOnUnitSphere{
                        pointOnUnitCube.x * sqrt(1.0f - (t.y + t.z) / 2.0f + (t.y * t.z) / 3),
                        pointOnUnitCube.y * sqrt(1.0f - (t.z + t.x) / 2.0f + (t.z * t.x) / 3),
                        pointOnUnitCube.z * sqrt(1.0f - (t.x + t.y) / 2.0f + (t.x * t.y) / 3)
                    };
                    position = pointOnUnitSphere;
                    normal = glm::normalize(pointOnUnitSphere);

                    if (x != resolution_ && y != resolution_) {
                        indices[triIndex++] = i;
                        indices[triIndex++] = i + resolution_ + 2;
                        indices[triIndex++] = i + resolution_ + 1;

                        indices[triIndex++] = i;
                        indices[triIndex++] = i + 1;
                        indices[triIndex++] = i + resolution_ + 2;
                    }
                }
            }

            mesh_.create();
        }

        inline void draw() const {
            mesh_.draw();
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
        for (const auto& face: terrainFaces_) {
            face.draw();
        }
    }


    u32 resolution_{ 2 };
    std::array<Face, 6> terrainFaces_;
};