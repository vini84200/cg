//
// Created by vini84200 on 5/19/23.
//

#ifndef TRIANGLE_WINDOW_H
#define TRIANGLE_WINDOW_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, ColorBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0, vColor = 1 };
const GLuint NumVertices = 6;

#define BUFFER_OFFSET(a) ((void*)(a))

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

    // TODO: This is temporary and the buffers should be moved to a new class responsible for rendering, when this gets
    //  complex enough.

    /// OpenGL VAOs
    GLuint vertexArrayIds_[VAO_IDs::NumVAOs];

    /// OpenGL Buffers
    GLuint bufferIds_[Buffer_IDs::NumBuffers];

};


#endif //TRIANGLE_WINDOW_H
