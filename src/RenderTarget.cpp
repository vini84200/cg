//
// Created by vini84200 on 5/24/23.
//

#include "RenderTarget.h"
#include "imgui.h"
#include "loadShader/LoadShaders.h"
#include "Object.h"
#include <GL/gl.h>
#include <vector>

RenderTarget::RenderTarget() {
    currentConfig = RenderTargetConfig();
    // Load shaders
    ShaderInfo shaders[] =
            {
                    {GL_VERTEX_SHADER,   "assets/shaders/quad.vert"},
                    {GL_FRAGMENT_SHADER, "assets/shaders/quad.frag"},
                    {GL_NONE,            nullptr}
            };
    program = LoadShaders(shaders);

    float vertex_data[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
                           1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f};
    float texture_coords[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                              1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f};

    glGenVertexArrays(NumVAOs, &vao_[0]);

    glGenBuffers(NumBuffers, &vbo_[0]);

    glBindVertexArray(vao_[ModelSpace]);
    // Position
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[PositionBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
    // Texture
    glBindBuffer(GL_ARRAY_BUFFER, vbo_[TextureCoordBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
    glVertexAttribPointer(vTextureCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vTextureCoord);

    glBindVertexArray(0);

    glGenTextures(1, &texture);
    glGenTextures(1, &depth_texture);
}

void RenderTarget::init(RenderTargetConfig config) {
    currentConfig = config;
    pixels.resize(config.width * config.height);
    depthPixels.resize(config.width * config.height);
    currentConfig.capacity = config.width * config.height;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, currentConfig.width, currentConfig.height);

    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, currentConfig.width, currentConfig.height);

}

void RenderTarget::activate() {

}

void RenderTarget::deactivate() {

}

void RenderTarget::onResize(int width, int height) {
    currentConfig.width = width;
    currentConfig.height = height;

    if (currentConfig.requiredCapacity() > currentConfig.capacity) {
        currentConfig.capacity = currentConfig.requiredCapacity();
        pixels.resize(currentConfig.capacity);
        depthPixels.resize(currentConfig.capacity);
    }

    glDeleteTextures(1, &texture);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, currentConfig.width, currentConfig.height);
    
    glDeleteTextures(1, &depth_texture);
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, currentConfig.width, currentConfig.height);

}

void RenderTarget::render() {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexSubImage2D(GL_TEXTURE_2D,
                    0, 0, 0,
                    currentConfig.width, currentConfig.height,
                    GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(program);
    glBindVertexArray(vao_[ModelSpace]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program, "ourTexture"), 0);
    glDisable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glUseProgram(0);
}

glm::mat4 RenderTarget::getViewportMatrix() const {
    return glm::mat4 (
            glm::vec4(currentConfig.width / 2.0f, 0.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, -currentConfig.height / 2.0f, 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
            glm::vec4(currentConfig.width / 2.0f, currentConfig.height / 2.0f, 0.0f, 1.0f)
            );
}

void RenderTarget::renderImGui() {
    ImGui::Text("Size: %d x %d", currentConfig.width, currentConfig.height);
    ImGui::Text("Capacity: %d", currentConfig.capacity);
    if (ImGui::TreeNode("Depth Buffer")) {
        std::vector<float> depthPixels_;
        depthPixels_.reserve(depthPixels.size());
        ImGui::SliderFloat("Start", &currentConfig.depthBufferStart, -1.0f, 1.0f);
        ImGui::SliderFloat("End", &currentConfig.depthBufferEnd, -1.0f, 1.0f);

        // Render the depth buffer into the window
        for (auto pixel : depthPixels) {
            depthPixels_.push_back((pixel - currentConfig.depthBufferStart) / (currentConfig.depthBufferEnd - currentConfig.depthBufferStart));
        }

        // Render the depth buffer into a texture
        glBindTexture(GL_TEXTURE_2D, depth_texture);
        glTexSubImage2D(GL_TEXTURE_2D,
                        0, 0, 0,
                        currentConfig.width, currentConfig.height,
                        GL_RED, GL_FLOAT, depthPixels_.data());
        glBindTexture(GL_TEXTURE_2D, 0);

        ImGui::Image(ImTextureID (depth_texture), ImVec2 (200, 100));

        ImGui::TreePop();
    }

}

ColorPixel *RenderTarget::getPixelData() const {
    return const_cast<ColorPixel *>(pixels.data());
}

int RenderTarget::getWidth() const {
    return currentConfig.width;
}

int RenderTarget::getHeight() const {
    return currentConfig.height;
}

std::vector<DepthPixel> RenderTarget::getDepthBuffer() {
    return depthPixels;
}
