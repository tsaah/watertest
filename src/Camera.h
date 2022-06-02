#pragma once

#include "e_type.h"

class Camera final {
public:
    inline Camera () {
        update();
    }

    void setPosition(glm::vec3 v) {
        position_ = {v, 0};
        update();
    }

    void setTarget(glm::vec3 v) {
        setDirection(v.xyz() - position_.xyz());
    }

    void setDirection(glm::vec3 v) {
        direction_ = { glm::normalize(v), 0 };
        update();
    }

    void moveForward(f32 v) {
        position_ += direction_ * v;
    }
    void moveRight(f32 v) {
        const glm::vec4 right = { glm::normalize(glm::cross(up_.xyz(), direction_.xyz())), 0 };
        position_ += right * v;
    }

    void turn(f32 p, f32 y) {
        yaw_ += y;
        pitch_ += p;
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
        direction_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        direction_.y = sin(glm::radians(pitch_));
        direction_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        direction_ = { glm::normalize(direction_.xyz()), 0 };
    }

    void setFov(f32 v) {
        fov_ = v;
        update();
    }

    void setSize(i32 w, i32 h) {
        width_ = w;
        height_ = h;
        aspectRatio_ = static_cast<f32>(w) / static_cast<f32>(h);
        update();
    }

    void update() {
        direction_ = glm::normalize(direction_);
        view_ = glm::lookAt(position_.xyz(), direction_.xyz() + position_.xyz(), up_.xyz());
        projection_ = glm::perspective(glm::radians(fov_), aspectRatio_, near_, far_);

    }

    void store() {

    }

    glm::mat4 view_;
    glm::mat4 projection_;

    const glm::vec4 up_{ 0, 1, 0, 0 };
    glm::vec4 position_{ 0, 0, 0, 0 };
    glm::vec4 direction_{ 0, 0, 1, 0 };

    f32 fov_{ 90.0f };
    f32 aspectRatio_{ 1.0f };
    f32 near_{ 0.001f };
    f32 far_{ 1000.0f };
    f32 pitch_{ 0.0f };
    f32 yaw_{ -90.0f };
    i32 width_{ 100 };
    i32 height_{ 100 };

};