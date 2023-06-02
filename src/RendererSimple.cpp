//
// Created by vini84200 on 5/24/23.
//

#include "RendererSimple.h"
#include "loadShader/LoadShaders.h"

RendererSimple::RendererSimple() {
    ShaderInfo  shaders[] =
            {
                    { GL_VERTEX_SHADER, "assets/shaders/triangles.vert" },
                    { GL_FRAGMENT_SHADER, "assets/shaders/triangles.frag" },
                    { GL_NONE, nullptr }
            };
    program = LoadShaders( shaders );
}

void RendererSimple::render(Scene *scene, Camera *camera) {
    // Initialize the program
    glUseProgram( program );

    // Initialize the camera



    for (auto object : scene->getObjects()) {
        // Render the object
        renderObject(object);
    }
}

void RendererSimple::renderObject(std::shared_ptr<Object> object) {
    // Bind the VAO
    glBindVertexArray(object->getVaOs()[0]);

    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, object->getNumVertices());
    // TODO: use the indecesType to draw the triangles

    // Unbind the VAO
    glBindVertexArray(0);
}
