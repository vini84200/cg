//
// Created by vini84200 on 5/19/23.
//

#ifndef TRIANGLE_WINDOW_H
#define TRIANGLE_WINDOW_H

#include "Camera.h"
#include "Object.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Renderer.h"
#include <memory>
#include <vector>

const GLuint NumVertices = 6;


/**
 * This class is a wrapper for GLFWwindow.
 * It is responsible for creating the window and the OpenGL context.
 * It also handles the main loop for drawing and updating the scene.
 */
class Window {
public:

    /**
     * Creates a window.
     */
    Window();

    /**
     * Destroys the window.
     * This also terminates GLFW.
     */
    ~Window();

    /**
     * Runs the main loop.
     */
    void run();

private:
    void initialize();
    void update();
    void render();
    void terminate();

    /**
     * Callback for errors in GLFW, handles errors and throws exceptions.
     * @param error The error code.
     * @param description The error description.
     */
    static void onError(int error, const char* description);

    GLFWwindow* window_; ///< The GLFW window

    std::unique_ptr<Renderer> renderer_; ///< The renderer
    std::unique_ptr<Camera> camera_; ///< The camera
    std::unique_ptr<Scene> scene_; ///< The scene
};


#endif //TRIANGLE_WINDOW_H
