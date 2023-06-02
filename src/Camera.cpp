//
// Created by vini84200 on 5/24/23.
//

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(glm::vec3(pos), glm::vec3(pos + forward), glm::vec3(0, 1, 0));
}

Camera::Camera() {
}
