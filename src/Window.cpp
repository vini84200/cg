//
// Created by vini84200 on 5/19/23.
//

#include <cstdio>
#include <stdexcept>
#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "loadShader/LoadShaders.h"

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
    printf("Initializing...\n");
    // Create vertex array object
    glGenVertexArrays( NumVAOs, vertexArrayIds_ );
    glBindVertexArray( vertexArrayIds_[Triangles] );

    ShaderInfo  shaders[] =
            {
                    { GL_VERTEX_SHADER, "assets/shaders/triangles.vert" },
                    { GL_FRAGMENT_SHADER, "assets/shaders/triangles.frag" },
                    { GL_NONE, nullptr }
            };

    GLuint program = LoadShaders( shaders );
    glUseProgram( program );

    GLfloat  vertices[NumVertices][2] = {
            { -0.90f, -0.90f }, {  0.90, -0.90f }, { -0.f,  0.85f },  // Triangle 1
    };

    glGenBuffers( NumBuffers, bufferIds_ );
    glBindBuffer( GL_ARRAY_BUFFER, bufferIds_[ArrayBuffer] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices),
                  vertices, GL_STATIC_DRAW );

    glVertexAttribPointer( vPosition, 2, GL_FLOAT,
                           GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vPosition );

    GLfloat colors[NumVertices][3] = {
            { 1, 0, 0}, { 0, 1, 0}, { 0, 0, 1},
            { 1, 1, 0}, { 1, 0, 1}, { 0, 1, 1}
    };
    glBindBuffer( GL_ARRAY_BUFFER, bufferIds_[ColorBuffer] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(colors),
                  colors, GL_STATIC_DRAW );

    glVertexAttribPointer( vColor, 3, GL_FLOAT,
                           GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vColor );

    printf("Initialized\n");
}

void Window::update() {

}

void Window::render() {
    glClear( GL_COLOR_BUFFER_BIT );

    glBindVertexArray( vertexArrayIds_[Triangles] );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );

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

