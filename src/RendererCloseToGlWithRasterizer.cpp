//
// Created by vini84200 on 6/16/23.
//

#include "RendererCloseToGlWithRasterizer.h"
#include "C2GLProgram.h"
#include "GouradProgram.h"
#include "Object.h"
#include "PhongProgram.h"
#include "Rasterizer.h"
#include "imgui.h"
#include "GLFW/glfw3.h"
#include <tracy/Tracy.hpp>

RendererCloseToGlWithRasterizer::RendererCloseToGlWithRasterizer() {
    program = new GouradProgram();

}

void RendererCloseToGlWithRasterizer::render(Scene *scene, Camera *camera) {
    // Initialize the camera
    ZoneScoped;
    FrameMarkStart("RendererCloseToGlWithRasterizer::render");
    renderTarget.Clear({0, 0, 0, 1});
    glm::mat4 projectionMatrix = camera->getProjectionMatrix();
    glm::mat4 viewMatrix = camera->getViewMatrix();

    switch (renderType) {
        case TRIANGLES:
            rasterizer.setMode(Rasterizer::Solid);
            break;
        case LINES:
            rasterizer.setMode(Rasterizer::Wireframe);
            break;
        case POINTS:
            rasterizer.setMode(Rasterizer::Point);
            break;
    }
    rasterizer.setRenderTarget(&renderTarget);

    rasterizer.setCcw(ccw_);
    rasterizer.setBackfaceCulling(backFaceCulling_);
    
    C2GLProgram &program = *this->program;
    rasterizer.setProgram(program);
    program.setViewMatrix(viewMatrix);
    program.setProjectionMatrix(projectionMatrix);
    program.setViewPos(camera->getPos());

    for (auto object : scene->getObjects()) {
        // Render the object
        renderObject(object);
    }


    // Render the Render Target
    renderTarget.render();
//    FrameImage(renderTarget.getPixelData(), renderTarget.getWidth(), renderTarget.getHeight(), 0, 1);
    FrameMarkEnd("RendererCloseToGlWithRasterizer::render");
}

void RendererCloseToGlWithRasterizer::renderObject(std::shared_ptr<Object> object) {

    // TODO: Use the object's model matrix
    rasterizer.getProgram().setModelMatrix(glm::mat4 (1.0f));

    if (colorOverride_) {
        Material m = Material(
                newColor,
                glm::vec3(0.0f),
                glm::vec3(0.0f),
                10.0f
                );
        rasterizer.getProgram().setMaterial(m);

    } else {
        Material *m = object->getMaterial(0);

        if (m == nullptr) {
            m = new Material(
                    glm::vec3(1.0f),
                    glm::vec3(0.0f),
                    glm::vec3(0.0f),
                    10.0f
            );
        }

        if (m->shine <= 0.0f)
            m->specular = glm::vec3(0.0f);

        rasterizer.getProgram().setMaterial(*m);
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


    if (ImGui::Button("Choose Shading and Ilumination Model")) {
        ImGui::OpenPopup("Shading - Ilumination Model");
    }

    if (ImGui::BeginPopup("Shading - Ilumination Model")) {
        if (ImGui::Selectable("Gourad - Phong")) {
            delete program;
            program = new GouradProgram();
        }
        if (ImGui::Selectable("Phong - Phong")) {
            delete program;
            program = new PhongProgram();
        }
        ImGui::EndPopup();
    }
    
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
