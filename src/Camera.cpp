//
// Created by vini84200 on 5/24/23.
//

#include "Camera.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
    fov  = 45.0f;
    pos  = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    aspectRatio = 9.f/16.f;
    near = 0.1f;
    far = 1000.f;
}
glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(glm::vec3(pos), glm::vec3(pos + forward), glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(fov), aspectRatio,near,far);
}

void Camera::renderImGui() {
    ImGui::Begin("Camera");
    ImGui::SliderFloat("fov", &fov, 0.0f, 180.0f);
    ImGui::SliderFloat("aspectRatio", &aspectRatio, 0.0f, 10.0f);
    ImGui::SliderFloat("near", &near, 0.0f, 10.0f);
    ImGui::SliderFloat("far", &far, 0.0f, 1000.0f);
    ImGui::End();
}
