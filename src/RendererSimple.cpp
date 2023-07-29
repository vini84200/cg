//
// Created by vini84200 on 5/24/23.
//

#include "RendererSimple.h"
#include "Object.h"
#include "imgui.h"
#include "loadShader/LoadShaders.h"
#include <cstdio>

RendererSimple::RendererSimple() {
    ShaderInfo shaders[] = {
        {  GL_VERTEX_SHADER, "assets/shaders/triangles.vert"},
        {GL_FRAGMENT_SHADER, "assets/shaders/triangles.frag"},
        {           GL_NONE,                         nullptr}
    };
    program = LoadShaders(shaders);

    lightModelVSLocation = glGetSubroutineUniformLocation(
        program, GL_VERTEX_SHADER, "lightModelVS");

    if (lightModelVSLocation == -1) {
        printf("Error: Can't find LightModel uniform location in "
               "vertex shader\n");
    }

    shaderSubroutineIndicesVS[GOURAD_AD] = glGetSubroutineIndex(
        program, GL_VERTEX_SHADER, "gouroudAD");
    shaderSubroutineIndicesVS[GOURAD_SPEC] = glGetSubroutineIndex(
        program, GL_VERTEX_SHADER, "gouroudADS");
    shaderSubroutineIndicesVS[PHONG]
        = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "phong");
    shaderSubroutineIndicesVS[NO_SHADE]
        = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "noShade");

    lightModelFSLocation = glGetSubroutineUniformLocation(
        program, GL_FRAGMENT_SHADER, "LightModelFS");
    shaderSubroutineIndicesFS[GOURAD_AD] = glGetSubroutineIndex(
        program, GL_FRAGMENT_SHADER, "gouroudAD");
    shaderSubroutineIndicesFS[GOURAD_SPEC] = glGetSubroutineIndex(
        program, GL_FRAGMENT_SHADER, "gouroudADS");
    shaderSubroutineIndicesFS[PHONG]
        = glGetSubroutineIndex(program, GL_FRAGMENT_SHADER, "phong");
    shaderSubroutineIndicesFS[NO_SHADE] = glGetSubroutineIndex(
        program, GL_FRAGMENT_SHADER, "noShade");
    assert(lightModelFSLocation != -1);
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
    glUseProgram(program);

    // Initialize the camera
    glm::mat4 projectionMatrix = camera->getProjectionMatrix();
    glm::mat4 viewMatrix       = camera->getViewMatrix();

    // Set the projection matrix
    glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1,
                       GL_FALSE, &projectionMatrix[0][0]);
    // Set the view matrix
    glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1,
                       GL_FALSE, &viewMatrix[0][0]);
    glUniform3fv(glGetUniformLocation(program, "viewPos"), 1,
                 &camera->getPos()[0]);

    // TODO: This is temp
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1,
                       GL_FALSE, &glm::mat4(1.0f)[0][0]);


    for (auto object : scene->getObjects()) {
        // Render the object
        renderObject(object);
    }
}

void RendererSimple::renderObject(std::shared_ptr<Object> object) {
    // Bind the VAO
    glBindVertexArray(object->getVaOs()[ModelSpace]);

    // Draw the triangles
    for (const auto &callSpan : object->getCallSpan()) {
        // Setup the material
        if (colorOverride_) {
            glUniform3fv(glGetUniformLocation(
                             program, "material.ambientColor"),
                         1, &newColor[0]);
            glUniform3fv(glGetUniformLocation(
                             program, "material.diffuseColor"),
                         1, &newColor[0]);
            glUniform3fv(glGetUniformLocation(
                             program, "material.specularColor"),
                         1, &newColor[0]);
            glUniform1f(
                glGetUniformLocation(program, "material.shine"), 32);

        } else {
            Material *m = object->getMaterial(callSpan.materialIndex);
            glUniform3fv(glGetUniformLocation(
                             program, "material.ambientColor"),
                         1, &m->ambient[0]);
            glUniform3fv(glGetUniformLocation(
                             program, "material.diffuseColor"),
                         1, &m->diffuse[0]);
            if (m->shine <= 0) {
                glUniform3f(glGetUniformLocation(
                                program, "material.specularColor"),
                            0, 0, 0);
                glUniform1f(
                    glGetUniformLocation(program, "material.shine"),
                    m->shine);
            } else {
                glUniform3fv(glGetUniformLocation(
                                 program, "material.specularColor"),
                             1, &m->specular[0]);
                glUniform1f(
                    glGetUniformLocation(program, "material.shine"),
                    m->shine);
            }

            if (m->diffTexture.has_value()) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m->diffTexture.value().getTextureIndex());
                glUniform1i(glGetUniformLocation(program, "material.diffTexture"), 0);
                glUniform1i(glGetUniformLocation(program, "material.hasDiffTexture"), 1);
            } else {
                glUniform1i(glGetUniformLocation(program, "material.hasDiffTexture"), 0);
            }
        }

        GLsizei subroutineLocations
            = GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS;
        //        glGetIntegerv(GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS,
        //        &subroutineLocations); assert(subroutineLocations !=
        //        -1); assert(subroutineLocations >=
        //        lightModelVSLocation);


        GLuint subroutineIndicesVS[subroutineLocations];
        GLuint subroutineIndicesFS[subroutineLocations];

        subroutineIndicesVS[lightModelVSLocation]
            = shaderSubroutineIndicesVS[shaderSubroutine];
        subroutineIndicesFS[lightModelFSLocation]
            = shaderSubroutineIndicesFS[shaderSubroutine];

        assert(shaderSubroutineIndicesVS[shaderSubroutine] != -1);
        assert(lightModelVSLocation == 0);
        glUniformSubroutinesuiv(
            GL_VERTEX_SHADER, 1,
            &shaderSubroutineIndicesVS[shaderSubroutine]);

        assert(shaderSubroutineIndicesFS[shaderSubroutine] != -1);
        assert(lightModelFSLocation == 0);
        glUniformSubroutinesuiv(
            GL_FRAGMENT_SHADER, 1,
            &shaderSubroutineIndicesFS[shaderSubroutine]);


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
    ImGui::RadioButton("ModelSpace", (int *)&renderType, TRIANGLES);
    ImGui::RadioButton("Lines", (int *)&renderType, LINES);
    ImGui::RadioButton("Points", (int *)&renderType, POINTS);

    ImGui::Checkbox("Back Face Culling", &backFaceCulling_);
    ImGui::Checkbox("CCW", &ccw_);

    ImGui::Checkbox("##ColorOverride", &colorOverride_);
    ImGui::SameLine();
    ImGui::ColorEdit3("Color", &newColor[0]);

    std::string shaderSubroutineName[NUM_SUBROUTINES];
    shaderSubroutineName[GOURAD_AD]   = "Gourad AD";
    shaderSubroutineName[GOURAD_SPEC] = "Gourad ADS";
    shaderSubroutineName[PHONG]       = "Phong";
    shaderSubroutineName[NO_SHADE]    = "No Shade";

    ImGui::Text("Shader Subroutine: %s",
                shaderSubroutineName[shaderSubroutine].c_str());
    if (ImGui::Button("Choose Subroutine")) {
        ImGui::OpenPopup("Subroutine");
    }

    if (ImGui::BeginPopup("Subroutine")) {
        if (ImGui::Selectable("Gourad AD")) {
            shaderSubroutine = GOURAD_AD;
        }
        if (ImGui::Selectable("Gourad ADS")) {
            shaderSubroutine = GOURAD_SPEC;
        }
        if (ImGui::Selectable("Phong")) {
            shaderSubroutine = PHONG;
        }
        if (ImGui::Selectable("No Shade")) {
            shaderSubroutine = NO_SHADE;
        }
        ImGui::EndPopup();
    }


    ImGui::End();
}

void RendererSimple::update(float dt) {}

std::string RendererSimple::getName() const {
    return "RendererSimple";
}
