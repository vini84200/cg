//
// Created by vini84200 on 5/24/23.
//

#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "imgui.h"
#include "GLFW/glfw3.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/epsilon.hpp>

Camera::Camera() {
    fov = 45.0f;
    pos = glm::vec4(0.0f, 0.0f, 400.0f, 1.0f);
    forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    aspectRatio = 800.0f / 600.0f;
    near = 0.1f;
    far = 100000.f;
}

glm::mat4 Camera::getViewMatrix() {
    glm::vec3 up (0.0f, 1.0f, 0.0f);
    glm::vec3 f = glm::normalize(glm::vec3(forward));
    glm::vec3 right = glm::normalize(glm::cross(f, up));
    up = glm::cross(right, f);

    glm::mat4 view(
            right.x, up.x, -forward.x, 0,
            right.y, up.y, -forward.y, 0,
            right.z, up.z, -forward.z, 0,
            -glm::dot(glm::vec3(right), glm::vec3(pos)),
            -glm::dot(glm::vec3(up), glm::vec3(pos)),
            glm::dot(glm::vec3(f), glm::vec3(pos)),
            1
    );
//    glm::mat4 obj = glm::lookAt(glm::vec3(pos), glm::vec3(pos + forward), glm::vec3(0, 1, 0));
//
//    assert(glm::all(glm::epsilonEqual(view[0], obj[0], 0.001f)));
//    assert(glm::all(glm::epsilonEqual(view[1], obj[1], 0.001f)));
//    assert(glm::all(glm::epsilonEqual(view[2], obj[2], 0.001f)));
//    assert(glm::all(glm::epsilonEqual(view[3], obj[3], 0.001f)));


    return view;
}

glm::mat4 Camera::getProjectionMatrix() {
    float radianFov = glm::radians(fov);
//    return glm::perspective(glm::radians(fov), aspectRatio, near, far);
    glm::mat4 proj = glm::mat4(
            1.0f / (aspectRatio * glm::tan(radianFov / 2.0f)), 0, 0, 0,
            0, 1.0f / glm::tan(radianFov / 2.0f), 0, 0,
            0, 0, -(far + near) / (far - near), -1,
            0, 0, -(2.0f * far * near) / (far - near), 0
    );

//    glm::mat4 obj = glm::perspective(glm::radians(fov), aspectRatio, near, far);
//
//    assert(glm::all(glm::epsilonEqual(proj[0], obj[0], 0.001f)));
//    assert(glm::all(glm::epsilonEqual(proj[1], obj[1], 0.001f)));
//    assert(glm::all(glm::epsilonEqual(proj[2], obj[2], 0.001f)));
//    assert(glm::all(glm::epsilonEqual(proj[3], obj[3], 0.001f)));

    return proj;
}

void Camera::renderImGui() {
    ImGui::Begin("Camera");
    ImGui::SliderFloat("fov", &fov, 0.0f, 180.0f);
    ImGui::SliderFloat("aspectRatio", &aspectRatio, 0.0f, 10.0f);
    ImGui::SliderFloat("near", &near, 0.0f, 10.0f);
    ImGui::SliderFloat("far", &far, 0.0f, 100000.0f);
    ImGui::Checkbox("is_orbital", &is_orbital_);

    if (ImGui::Button("Reset")) {
        pos = glm::vec4(0.0f, 0.0f, 400.0f, 1.0f);
        forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
    }
    if (ImGui::Button("Reset movement")) {
        movement_ = glm::vec3(0.0f);
        speed_up_ = false;
    }
    if (ImGui::TreeNode("pos")) {
        ImGui::Text("x: %f", pos.x);
        ImGui::Text("y: %f", pos.y);
        ImGui::Text("z: %f", pos.z);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("View Matrix")) {
        glm::mat4 view = getViewMatrix();
        ImGui::Text(" %f  %f  %f  %f", view[0][0], view[1][0], view[2][0], view[3][0]);
        ImGui::Text(" %f  %f  %f  %f", view[0][1], view[1][1], view[2][1], view[3][1]);
        ImGui::Text(" %f  %f  %f  %f", view[0][2], view[1][2], view[2][2], view[3][2]);
        ImGui::Text(" %f  %f  %f  %f", view[0][3], view[1][3], view[2][3], view[3][3]);

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Projection Matrix")) {
        glm::mat4 projection = getProjectionMatrix();
        ImGui::Text(" %f  %f  %f  %f", projection[0][0], projection[1][0], projection[2][0], projection[3][0]);
        ImGui::Text(" %f  %f  %f  %f", projection[0][1], projection[1][1], projection[2][1], projection[3][1]);
        ImGui::Text(" %f  %f  %f  %f", projection[0][2], projection[1][2], projection[2][2], projection[3][2]);
        ImGui::Text(" %f  %f  %f  %f", projection[0][3], projection[1][3], projection[2][3], projection[3][3]);

        ImGui::TreePop();
    }

    ImGui::End();
}

void Camera::onKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            movement_ += glm::vec3(0, 0, 1);
        } else if (action == GLFW_RELEASE) {
            movement_ -= glm::vec3(0, 0, 1);
        }
    }
    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            movement_ += glm::vec3(0, 0, -1);
        } else if (action == GLFW_RELEASE) {
            movement_ -= glm::vec3(0, 0, -1);
        }
    }

    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            movement_ += glm::vec3(-1, 0, 0);
        } else if (action == GLFW_RELEASE) {
            movement_ -= glm::vec3(-1, 0, 0);
        }
    }

    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            movement_ += glm::vec3(1, 0, 0);
        } else if (action == GLFW_RELEASE) {
            movement_ -= glm::vec3(1, 0, 0);
        }
    }

    if (key == GLFW_KEY_SPACE) {
        if (action == GLFW_PRESS) {
            movement_ += glm::vec3(0, 1, 0);
        } else if (action == GLFW_RELEASE) {
            movement_ -= glm::vec3(0, 1, 0);
        }
    }
    if (key == GLFW_KEY_LEFT_SHIFT) {
        if (action == GLFW_PRESS) {
            movement_ += glm::vec3(0, -1, 0);
        } else if (action == GLFW_RELEASE) {
            movement_ -= glm::vec3(0, -1, 0);
        }
    }
    if (key == GLFW_KEY_R) {
        if (action == GLFW_PRESS) {
            pos = glm::vec4(0.0f, 0.0f, 400.0f, 1.0f);
            forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        }
    }
    if (key == GLFW_KEY_Q) {
        if (action == GLFW_PRESS) {
            is_orbital_ = !is_orbital_;
        }
    }
    if (key == GLFW_KEY_E) {
        if (action == GLFW_PRESS) {
            movement_ = glm::vec3(0, 0, 0);
        }
    }
    if (key == GLFW_KEY_LEFT_CONTROL) {
        if (action == GLFW_PRESS) {
            speed_up_ = true;
        }
        if (action == GLFW_RELEASE) {
            speed_up_ = false;
        }
    }
}

void Camera::update(float dt) {
    glm::vec3 u = glm::normalize(glm::cross(glm::vec3(forward), glm::vec3(0, 1, 0)));
    glm::vec3 v = glm::normalize(glm::cross(u, glm::vec3(forward)));
    glm::vec3 w = glm::normalize(glm::vec3(forward));

    glm::mat4 toWorld = glm::mat4(u.x, u.y, u.z, 0,
                                  v.x, v.y, v.z, 0,
                                  w.x, w.y, w.z, 0,
                                  0, 0, 0, 1);
    float speed = speed_up_ ? 500.f : 100.f;
    pos += toWorld * glm::vec4(movement_ * dt * speed, 0);

    if (is_orbital_) {
        forward = -glm::vec4(glm::normalize(glm::vec3(pos)), 0);
    }
}

void Camera::onWindowResize(int width, int height) {
    aspectRatio = (float) width / (float) height;

}

void Camera::onMouseButton(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            is_mouse_pressed_ = true;
        } else if (action == GLFW_RELEASE) {
            is_mouse_pressed_ = false;
        }
    }
}

void Camera::onMouseMove(double xpos, double ypos) {

}

void Camera::onMouseMoveDelta(double dx, double dy) {
    if (is_mouse_pressed_) {
        glm::vec3 pra_frente = glm::normalize(glm::vec3(forward));
        glm::vec3 pro_lado = glm::normalize(glm::cross(glm::vec3(forward), glm::vec3(0, 1, 0)));
        glm::vec3 pra_cima = glm::normalize(glm::cross(pro_lado, glm::vec3(forward)));

        if (is_orbital_) {

        } else {
            float speed = speed_up_ ? 0.001f : 0.0005f;
            forward = glm::rotate(glm::mat4(1), (float) dx * speed, pra_cima) * forward;
            forward = glm::rotate(glm::mat4(1), (float) dy * speed, pro_lado) * forward;
        }

    }
}

glm::vec4 Camera::getCCSz() const {
    return glm::normalize(forward);
}

glm::vec4 Camera::getCCSy() const {
    glm::vec3 pro_lado = glm::normalize(glm::cross(glm::vec3(forward), glm::vec3(0, 1, 0)));
    return glm::normalize(glm::vec4(pro_lado, 0));
}

glm::vec4 Camera::getCCSx() const {
    glm::vec3 pro_lado = glm::normalize(glm::cross(glm::vec3(forward), glm::vec3(0, 1, 0)));
    glm::vec3 pra_cima = glm::normalize(glm::cross(pro_lado, glm::vec3(forward)));
    return glm::normalize(glm::vec4(pra_cima, 0));
}

const glm::vec4 &Camera::getPos() const {
    return pos;
}

void Camera::setPos(const glm::vec4 &pos) {
    Camera::pos = pos;
}

