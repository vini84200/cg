//
// Created by vini84200 on 6/16/23.
//

#include "RendererCloseToGlWithRasterizer.h"
#include "Object.h"
#include "Rasterizer.h"
#include "imgui.h"
#include "GLFW/glfw3.h"

RendererCloseToGlWithRasterizer::RendererCloseToGlWithRasterizer() {

}

void RendererCloseToGlWithRasterizer::render(Scene *scene, Camera *camera) {
    // Initialize the camera
    renderTarget.Clear({0, 0, 0, 1});
    glm::mat4 projectionMatrix = camera->getProjectionMatrix();
    glm::mat4 viewMatrix = camera->getViewMatrix();

    rasterizer.setMode(Rasterizer::Solid);
    rasterizer.setRenderTarget(&renderTarget);



    for (auto object : scene->getObjects()) {
        object->updateCameraVertices(projectionMatrix, viewMatrix);
        object->updateCameraVAO();
        // Render the object
        renderObject(object);
    }


    // Render the Render Target
    renderTarget.render();
}

void RendererCloseToGlWithRasterizer::renderObject(std::shared_ptr<Object> object) {

    for (auto &vertex : object->cameraVertices) {
        vertex.position = renderTarget.getViewportMatrix() * vertex.position;
    }

    if (colorOverride_) {
        glUniform3fv(glGetUniformLocation(program, "material.ambientColor"), 1, &newColor[0]);
        glUniform3fv(glGetUniformLocation(program, "material.diffuseColor"), 1, &newColor[0]);
        glUniform3fv(glGetUniformLocation(program, "material.specularColor"), 1, &newColor[0]);
        glUniform1f(glGetUniformLocation(program, "material.shine"), 0.3);

    } else {
        Material *m = object->getMaterial(0);
        glUniform3fv(glGetUniformLocation(program, "material.ambientColor"), 1, &m->ambient[0]);
        glUniform3fv(glGetUniformLocation(program, "material.diffuseColor"), 1, &m->diffuse[0]);
        glUniform3fv(glGetUniformLocation(program, "material.specularColor"), 1, &m->specular[0]);
        glUniform1f(glGetUniformLocation(program, "material.shine"), m->shine);
    }

    rasterizer.renderObject( object.get());
}

void RendererCloseToGlWithRasterizer::renderImGui() {
    ImGui::Begin("RendererCloseToGlWithRasterizer");
    ImGui::Text("Render Type");
    ImGui::RadioButton("Solid", (int *) &renderType, TRIANGLES);
    ImGui::RadioButton("Wireframe", (int *) &renderType, LINES);
    ImGui::RadioButton("Points", (int *) &renderType, POINTS);

    ImGui::Checkbox("Back Face Culling", &backFaceCulling_);
    ImGui::Checkbox("CCW", &ccw_);

    ImGui::Checkbox("##ColorOverride", &colorOverride_);
    ImGui::SameLine();
    ImGui::ColorEdit3("Color", &newColor[0]);

    ImGui::Text("Render Target");
    renderTarget.renderImGui();
    ImGui::End();
}

void RendererCloseToGlWithRasterizer::update(float dt) {

}

std::string RendererCloseToGlWithRasterizer::getName() const {
    return "RendererCloseToGlWithRasterizer";
}

void RendererCloseToGlWithRasterizer::start() {
    int width, height;
    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    renderTarget.init({width, height, 0});
}

void RendererCloseToGlWithRasterizer::pause() {
    renderTarget.deactivate();
}

void RendererCloseToGlWithRasterizer::resume() {
    renderTarget.activate();
}

void RendererCloseToGlWithRasterizer::onResize(int width, int height) {
    renderTarget.onResize(width, height);
}
