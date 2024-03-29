//
// Created by vini84200 on 5/19/23.
//

#include "Window.h"
#include "Camera.h"
#include "GLFW/glfw3.h"
#include "ObjectFromFileIn.h"
#include "RendererCloseToGl.h"
#include "RendererCloseToGlWithRasterizer.h"
#include "RendererSimple.h"
#include "TriangleObject.h"
#include "glad/glad.h"
#include "imgui.h"
#include "tracy/Tracy.hpp"
#include <cstdio>
#include <stdexcept>

Window::Window() {
    int success = glfwInit();
    if (!success)
        throw std::runtime_error("Failed to initialize GLFW");
    glfwSetErrorCallback(onError);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_
        = glfwCreateWindow(800, 600, "Triangle", nullptr, nullptr);

    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    char *version  = (char *)glGetString(GL_VERSION);
    char *vendor   = (char *)glGetString(GL_VENDOR);
    char *renderer = (char *)glGetString(GL_RENDERER);
    char *glslVersion
        = (char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("OpenGL version: %s\n", version);
    printf("Vendor: %s\n", vendor);
    printf("Renderer: %s\n", renderer);
    printf("GLSL version: %s\n", glslVersion);

    glfwSwapInterval(1); // Enable vsync
    vsync_ = true;

    glfwSetWindowUserPointer(window_, this);
    glfwSetKeyCallback(window_, [](GLFWwindow *window, int key,
                                   int scancode, int action,
                                   int mods) {
        Window *w = (Window *)glfwGetWindowUserPointer(window);
        w->onKeyPressed(key, scancode, action, mods);
    });
    glfwSetWindowSizeCallback(
        window_, [](GLFWwindow *window, int width, int height) {
            Window *w = (Window *)glfwGetWindowUserPointer(window);
            w->onWindowResized(width, height);
        });
    glfwSetMouseButtonCallback(
        window_,
        [](GLFWwindow *window, int button, int action, int mods) {
            Window *w = (Window *)glfwGetWindowUserPointer(window);
            w->onMouseButton(button, action, mods);
        });
    glfwSetCursorPosCallback(
        window_, [](GLFWwindow *window, double xpos, double ypos) {
            Window *w = (Window *)glfwGetWindowUserPointer(window);
            w->onMouseMove(xpos, ypos);
        });

    renderer_ = std::make_unique<RendererSimple>();
    camera_   = std::make_unique<Camera>();
    scene_    = std::make_unique<Scene>();
}

Window::~Window() {
    printf("Terminating GLFW\n");
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void Window::run() {
    initialize();

    float lastTime  = 0.0f;
    bool firstFrame = true;
    while (!glfwWindowShouldClose(window_)) {
        float time      = (float)glfwGetTime();
        float deltaTime = time - lastTime;
        lastTime        = time;
        if (firstFrame) {
            deltaTime  = 0.0f;
            firstFrame = false;
        } else {
            update(deltaTime);
        }
        render();
    }

    terminate();
}

void Window::initialize() {
    scene_->addObject(std::make_unique<TriangleObject>());
    //    scene_->addObject(std::make_unique<ObjectFromFileIn>("assets/cow_up.in"));

    imguiPlugin_.init();
    imguiPlugin_.setWindow(this);
}

void Window::update(float deltatime) {
    camera_->update(deltatime);
    renderer_->update(deltatime);
    scene_->update(deltatime);
    imguiPlugin_.update(deltatime);
}

void Window::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer_->render(scene_.get(), camera_.get());
    imguiPlugin_.render();


    glfwSwapBuffers(window_);
    FrameMark;
    glfwPollEvents();
}

void Window::terminate() { printf("Gracefully terminating\n"); }

void Window::onError(int error, const char *description) {
    fprintf(stderr, "GLFW Error: (%d) %s\n", error, description);
    throw std::runtime_error("GLFW Error");
}

GLFWwindow *Window::getWindow() const { return window_; }

void Window::setWindow(GLFWwindow *window) { window_ = window; }

void Window::renderImGuiMainWindow() {
    camera_->renderImGui();
    scene_->renderImGui();
    renderer_->renderImGui();

    ImGui::Begin("Window");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Time per frame: %.3f ms",
                1000.0f / ImGui::GetIO().Framerate);
    ImGui::Text("VSync: %s", vsync_ ? "On" : "Off");
    ImGui::SameLine();
    if (ImGui::Button("Toggle VSync")) {
        vsync_ = !vsync_;
        glfwSwapInterval(vsync_ ? 1 : 0);
    }

    ImGui::Text("Current renderer: %s", renderer_->getName().c_str());
    if (ImGui::Button("Change renderer")) {
        ImGui::OpenPopup("Change renderer");
    }
    if (ImGui::BeginPopup("Change renderer")) {
        char rendererName[256];
        sprintf(rendererName, "%s [Current]",
                renderer_->getName().c_str());
        if (ImGui::Selectable(rendererName), true) {
        }

        for (auto &renderer : renderers_) {
            sprintf(rendererName, "%s", renderer->getName().c_str());
            if (ImGui::Selectable(rendererName)) {
                printf("Changing renderer to %s\n",
                       renderer->getName().c_str());
                renderer_->pause();
                std::swap(renderer, renderer_);
                renderer_->resume();
            }
        }

        ImGui::Separator();
        if (ImGui::Selectable("New Simple")) {
            printf("Changing renderer to Simple\n");
            renderers_.emplace_back(std::move(renderer_));
            renderer_ = std::make_unique<RendererSimple>();
            renderer_->start();
        }
        if (ImGui::Selectable("New CloseToGl")) {
            printf("Changing renderer to CloseToGl\n");
            renderers_.emplace_back(std::move(renderer_));
            renderer_ = std::make_unique<RendererCloseToGl>();
            renderer_->start();
        }
        if (ImGui::Selectable("New CloseToGl With Rasterizer")) {
            printf(
                "Changing renderer to CloseToGl2 With Rasterizer\n");
            renderers_.emplace_back(std::move(renderer_));
            renderer_
                = std::make_unique<RendererCloseToGlWithRasterizer>();
            renderer_->start();
        }
        ImGui::EndPopup();
    }
    ImGui::End();
}

void Window::onKeyPressed(int key, int scancode, int action,
                          int mods) {
    ImGuiIO &io = ImGui::GetIO();

    if (io.WantCaptureKeyboard) {
        return;
    }


    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window_, true);
    if (key == GLFW_KEY_G && action == GLFW_PRESS)
        show_gui_ = !show_gui_;
    camera_->onKey(key, scancode, action, mods);
}

void Window::onWindowResized(int w, int h) {
    glViewport(0, 0, w, h);
    camera_->onWindowResize(w, h);
    renderer_->onResize(w, h);
}

void Window::onMouseButton(int button, int action, int mods) {
    ImGuiIO &io = ImGui::GetIO();

    if (io.WantCaptureMouse) {
        return;
    }

    camera_->onMouseButton(button, action, mods);
}

void Window::onMouseMove(double xpos, double ypos) {

    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;
    }


    camera_->onMouseMove(xpos, ypos);
    float dx = xpos - lastX_;
    float dy = ypos - lastY_;
    lastX_   = xpos;
    lastY_   = ypos;
    camera_->onMouseMoveDelta(dx, dy);
}
