//
// Created by vini84200 on 5/19/23.
//

#include <cstdio>
#include <stdexcept>
#include "RendererSimple.h"
#include "TriangleObject.h"
#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

Window::Window() {
    int success = glfwInit();
    if (!success)
        throw std::runtime_error("Failed to initialize GLFW");
    glfwSetErrorCallback(onError);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(800, 600, "Triangle", nullptr, nullptr);

    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }
    glfwMakeContextCurrent(window_);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    char *version = (char *) glGetString(GL_VERSION);
    char *vendor = (char *) glGetString(GL_VENDOR);
    char *renderer = (char *) glGetString(GL_RENDERER);
    char *glslVersion = (char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("OpenGL version: %s\n", version);
    printf("Vendor: %s\n", vendor);
    printf("Renderer: %s\n", renderer);
    printf("GLSL version: %s\n", glslVersion);

    glfwSwapInterval(1); // Enable vsync

    renderer_ = std::make_unique<RendererSimple>();
    camera_ = std::make_unique<Camera>();
    scene_ = std::make_unique<Scene>();
}

Window::~Window() {
    printf("Terminating GLFW\n");
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void Window::run() {
    initialize();

    while (!glfwWindowShouldClose(window_)) {
        update();
        render();
    }

    terminate();
}

void Window::initialize() {
    scene_->addObject(std::make_unique<TriangleObject>());

    imguiPlugin_.init();
    imguiPlugin_.setWindow(this);
}

void Window::update() {

}

void Window::render() {
    glClear( GL_COLOR_BUFFER_BIT );

    renderer_->render(scene_.get(), camera_.get());
    imguiPlugin_.render();

    glfwSwapBuffers(window_);
    glfwPollEvents();
}

void Window::terminate() {
    printf("Gracefully terminating\n");
}

void Window::onError(int error, const char *description) {
    fprintf(stderr, "GLFW Error: (%d) %s\n", error, description);
    throw std::runtime_error("GLFW Error");
}

GLFWwindow *Window::getWindow() const {
    return window_;
}

void Window::setWindow(GLFWwindow *window) {
    window_ = window;
}

void Window::renderImGuiMainWindow() {
    ImGui::Begin("Triangle");
    ImGui::Text("Hello, world!");
    camera_->renderImGui();
    scene_->renderImGui();
    renderer_->renderImGui();

    ImGui::End();
}
