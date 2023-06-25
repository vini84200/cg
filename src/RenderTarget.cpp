//
// Created by vini84200 on 5/24/23.
//

#include "RenderTarget.h"
#include "loadShader/LoadShaders.h"
#include "Object.h"
#include <GL/gl.h>

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
}

void RenderTarget::init(RenderTargetConfig config) {
    currentConfig = config;
    pixels.resize(config.width * config.height);
    depthPixels.resize(config.width * config.height);
    currentConfig.capacity = config.width * config.height;
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

}

void RenderTarget::render() {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 currentConfig.width,
                 currentConfig.height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                    pixels.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(program);
    glBindVertexArray(vao_[ModelSpace]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program, "ourTexture"), 0);
    glDisable(GL_CULL_FACE);
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


}
