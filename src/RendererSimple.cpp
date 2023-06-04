//
// Created by vini84200 on 5/24/23.
//

#include "RendererSimple.h"
#include "Object.h"
#include "loadShader/LoadShaders.h"
#include "imgui.h"
#include <GL/gl.h>
#include <math.h>

RendererSimple::RendererSimple() {
    ShaderInfo shaders[] =
            {
                    {GL_VERTEX_SHADER,   "assets/shaders/triangles.vert"},
                    {GL_FRAGMENT_SHADER, "assets/shaders/triangles.frag"},
                    {GL_NONE,            nullptr}
            };
    program = LoadShaders(shaders);
}

void RendererSimple::render(Scene *scene, Camera *camera) {
    // Set the configuration
    switch (renderType) {
        case POINTS:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        case LINES:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case TRIANGLES:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }

    if (backFaceCulling_) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        if (ccw_) {
            glFrontFace(GL_CCW);
        } else {
            glFrontFace(GL_CW);
        }
    } else {
        glDisable(GL_CULL_FACE);
    }
    glEnable(GL_DEPTH_TEST);

    // Initialize the program
    glUseProgram( program );

    // Initialize the camera
    glm::mat4 projectionMatrix = camera->getProjectionMatrix();
    glm::mat4 viewMatrix = camera->getViewMatrix();

    // Set the projection matrix
    glUniformMatrix4fv( glGetUniformLocation( program, "projection" ), 1, GL_FALSE, &projectionMatrix[0][0] );
    // Set the view matrix
    glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_FALSE, &viewMatrix[0][0] );

    // TODO: This is temp
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_FALSE, &glm::mat4(1.0f)[0][0] );


    for (auto object : scene->getObjects()) {
        // Render the object
        renderObject(object);
    }
}

void RendererSimple::renderObject(std::shared_ptr<Object> object) {
    // Bind the VAO
    glBindVertexArray(object->getVaOs()[0]);

    // Draw the triangles
    for (const auto& callSpan : object->getCallSpan()) {
        // Setup the material
        if (colorOverride_) {
            glUniform3fv(glGetUniformLocation(program, "material.ambientColor"), 1, &newColor[0]);
            glUniform3fv(glGetUniformLocation(program, "material.diffuseColor"), 1, &newColor[0]);
            glUniform3fv(glGetUniformLocation(program, "material.specularColor"), 1, &newColor[0]);
            glUniform1f(glGetUniformLocation(program, "material.shine"), 0.3);

        } else {
            Material *m = object->getMaterial(callSpan.materialIndex);
            glUniform3fv(glGetUniformLocation(program, "material.ambientColor"), 1, &m->ambient[0]);
            glUniform3fv(glGetUniformLocation(program, "material.diffuseColor"), 1, &m->diffuse[0]);
            glUniform3fv(glGetUniformLocation(program, "material.specularColor"), 1, &m->specular[0]);
            glUniform1f(glGetUniformLocation(program, "material.shine"), m->shine);
        }

        glDrawArrays(GL_TRIANGLES, callSpan.start, callSpan.count);
    }
//    glDrawArrays(GL_TRIANGLES, 0, object->getNumVertices());
    // TODO: use the indecesType to draw the triangles

    // Unbind the VAO
    glBindVertexArray(0);
}

void RendererSimple::renderImGui() {
    ImGui::Begin("RendererSimple");
    ImGui::Text("Render Type");
    ImGui::RadioButton("Triangles", (int *) &renderType, TRIANGLES);
    ImGui::RadioButton("Lines", (int *) &renderType, LINES);
    ImGui::RadioButton("Points", (int *) &renderType, POINTS);

    ImGui::Checkbox("Back Face Culling", &backFaceCulling_);
    ImGui::Checkbox("CCW", &ccw_);

    ImGui::Checkbox("##ColorOverride", &colorOverride_);
    ImGui::SameLine();
    ImGui::ColorEdit3("Color", &newColor[0]);
    ImGui::End();
}

void RendererSimple::update(float dt) {

}
