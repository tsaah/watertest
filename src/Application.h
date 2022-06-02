#pragma once

#include "e_type.h"
#include "Program.h"
#include "Buffer.h"
#include "UniformBuffer.h"
#include "VertexArrayObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "Planet.h"
#include "Texture2D.h"
#include "Quad.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <chrono>
#include <iostream>
#include <thread>

struct MM {
    Mesh m;
    VertexArrayObject vao;
    Buffer<glm::vec3> vbo;
    Buffer<u32> ibo;
};

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const *message, void const *user_param) {
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) { return; }

    auto const src_str = [source]() {
        switch (source) {
            case GL_DEBUG_SOURCE_API:
                return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION:
                return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER:
                return "OTHER";
            default:
                return "UNKNOWN SOURCE";
        }
    }();

    auto const type_str = [type]() {
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY:
                return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE:
                return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER:
                return "MARKER";
            case GL_DEBUG_TYPE_OTHER:
                return "OTHER";
            default:
                return "UNKNOWN TYPE";
        }
    }();

    auto const severity_str = [severity]() {
        switch (severity) {
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                return "NOTIFICATION";
            case GL_DEBUG_SEVERITY_LOW:
                return "LOW";
            case GL_DEBUG_SEVERITY_MEDIUM:
                return "MEDIUM";
            case GL_DEBUG_SEVERITY_HIGH:
                return "HIGH";
            default:
                return "UNKNOWN SEVERITY";
        }
    }();
    // std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}



// static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
// {
//     auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, GLFW_TRUE);
// }

class Application final {
public:
    Application(int argc, char** argv) {

    }

    int exec() {
        if (!init()) {
            return -1;
        }

        while (!glfwWindowShouldClose(window_)) {
            loop();
        }

        cleanup();

        return 0;
    }

    void resize(int width, int height) {
        if (window_ == nullptr) {
            return;
        }

        camera_.setSize(width, height);
    }

    void mouseButton(int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            previousMousePosition_ = currentMousePosition_;
            rightMouseDown_ = action == GLFW_PRESS;
        }
    }

    void mouseMove(f64 x, f64 y) {
        currentMousePosition_ = { x, y };
    }

    void key(int key, int scancode, int action, int mods) {
        switch (key) {
            case GLFW_KEY_W: {
                kw_ = action != GLFW_RELEASE;
            } break;
            case GLFW_KEY_A: {
                ka_ = action != GLFW_RELEASE;
            } break;
            case GLFW_KEY_S: {
                ks_ = action != GLFW_RELEASE;
            } break;
            case GLFW_KEY_D: {
                kd_ = action != GLFW_RELEASE;
            } break;
            default: break;
        }
    }

private:
    static void errorCallback(int error, const char* description) {
        std::cerr << "Error: " << description << std::endl;
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        app->key(key, scancode, action, mods);
    }

    static void sizeCallback(GLFWwindow* window, int width, int height) {
        auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        app->resize(width, height);
    }

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        app->mouseButton(button, action, mods);
    }

    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
        app->mouseMove(xpos, ypos);
    }

    b8 init() {
        glfwInit();
        glfwSetErrorCallback(&Application::errorCallback);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window_ = glfwCreateWindow(1400, 600, "Example", nullptr, nullptr);
        if (window_ == nullptr) {
            return false;
        }
        glfwSetWindowUserPointer(window_, this);
        glfwSetKeyCallback(window_, &Application::keyCallback);
        glfwSetWindowSizeCallback(window_, &Application::sizeCallback);
        glfwSetFramebufferSizeCallback(window_, &Application::framebufferSizeCallback);
        glfwSetCursorPosCallback(window_, &Application::cursorPositionCallback);
        glfwSetMouseButtonCallback(window_, &Application::mouseButtonCallback);

        glfwMakeContextCurrent(window_);
        glfwSwapInterval(1);

        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0) {
            return false;
        }

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEPTH_TEST);
        glDebugMessageCallback(message_callback, nullptr);
        glViewport(0, 0, 1400, 600);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window_, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
        ImGui::StyleColorsClassic();

        program_.create({
            Shader{ "shaders/default.vert", Shader::Type::Vertex },
            Shader{ "shaders/default.frag", Shader::Type::Fragment }
        });



        camera_.far_ = 10000;
        camera_.near_ = 1;
        camera_.setPosition({ 0, 0, -10000 });
        camera_.setTarget({ 0, 0, 0 });
        camera_.setSize(1400, 600);

        GLuint binding = 0;
        GLuint location = glGetUniformBlockIndex(program_, "CameraBlock");
        glUniformBlockBinding(program_, location, binding);
        cameraUniformBlock_.create(binding, camera_);

        glClearColor(0.29f, 0.3f, 0.38f, 1.0f);



        glPointSize(5.0f);

        quad_.create();
        planet_.init(2000);

        t_.load("textures/gebco_2021_tid_n0.0_s-90.0_w-90.0_e0.0.tif");

        return true;
    }

    void loop() {
        dt_ = 0.016f;
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program_.bind();
        cameraUniformBlock_.bind();
        t_.bind(0);
        const auto tex0Location = glGetUniformLocation(program_, "tex0");
        glUniform1i(tex0Location, 0);
        const auto seaLevelLocation = glGetUniformLocation(program_, "seaLevel");
        glUniform1f(seaLevelLocation, seaLevel_);

        planet_.draw();
        // quad_.draw();

        cameraUniformBlock_.unbind();
        program_.unbind();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        static bool showDemo = false;
        ImGui::Begin("Example");
        if (ImGui::Button("Show/Hide ImGui demo"))
            showDemo = !showDemo;
        ImGui::SliderFloat3("camera position", &camera_.position_.x, -2.0f, 2.0f);
        ImGui::SliderFloat3("camera direction", &camera_.direction_.x, -2.0f, 2.0f);
        ImGui::SliderFloat("camera near", &camera_.near_, 0.01f, 1.0f);
        ImGui::SliderFloat("camera far", &camera_.far_, 10.0f, 10000.0f);
        ImGui::SliderFloat("camera fov", &camera_.fov_, 10.0f, 170.0f);
        ImGui::Checkbox("Wireframe", &wireframe_);
        ImGui::SliderFloat("mouse sensitivity", &mouseSensitivity_, 0.1f, 2.0f);
        ImGui::SliderFloat("camera move speed", &cameraMoveSpeed_, 1.0f, 5000.0f);
        ImGui::SliderFloat("sea level", &seaLevel_, 0.0f, 1.0f);

        if (wireframe_) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        } else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        const auto cameraMoveAmmount = cameraMoveSpeed_ * dt_;
        if (kw_) {
            camera_.moveForward(cameraMoveAmmount);
        }
        if (ks_) {
            camera_.moveForward(-cameraMoveAmmount);
        }
        if (ka_) {
            camera_.moveRight(cameraMoveAmmount);
        }
        if (kd_) {
            camera_.moveRight(-cameraMoveAmmount);
        }


        if (rightMouseDown_) {
            f32 yaw = currentMousePosition_.x - previousMousePosition_.x;
            f32 pitch = previousMousePosition_.y - currentMousePosition_.y;
            previousMousePosition_ = currentMousePosition_;
            camera_.turn(pitch * mouseSensitivity_, yaw * mouseSensitivity_);
        }


        camera_.update();

        cameraUniformBlock_.setData(camera_);

        ImGui::End();
        if (showDemo)
            ImGui::ShowDemoWindow(&showDemo);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    void cleanup() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window_);
        window_ = nullptr;

        glfwTerminate();
    }

    GLFWwindow* window_{ nullptr };
    Program program_;
    Camera camera_;
    UniformBuffer cameraUniformBlock_;

    b8 wireframe_{ false };

    glm::vec2 currentMousePosition_;
    glm::vec2 previousMousePosition_;
    glm::vec2 mouseMoveDelta_;
    b8 rightMouseDown_{ false };

    b8 kw_{ false };
    b8 ka_{ false };
    b8 ks_{ false };
    b8 kd_{ false };

    f32 dt_{ 0.0f };

    f32 cameraMoveSpeed_{ 6.5f };
    f32 mouseSensitivity_{ 0.3f };

    Quad quad_;
    Planet planet_;
    Texture2D t_;

    f32 seaLevel_{ 0.5f };
};
