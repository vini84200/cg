//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_CAMERA_H
#define TRIANGLE_CAMERA_H

#include "glm/glm.hpp"

class Camera {


public:
    Camera();
    void renderImGui();

    glm::mat4 getProjectionMatrix();

    glm::mat4 getViewMatrix();
    void onKey(int key, int scancode, int action, int mods);
    void update(float dt);
    void onWindowResize(int width, int height);
    void onMouseButton(int button, int action, int mods);
    void onMouseMove(double xpos, double ypos);
    void onMouseMoveDelta(double dx, double dy);

protected:
    glm::vec4 pos{};
    glm::vec4 forward{};

    float fov;
    float near;
    float far;
    float aspectRatio;


    glm::vec3 movement_{};
    bool is_orbital_ = true;
    bool is_mouse_pressed_ = false;
    bool speed_up_{};
};


#endif //TRIANGLE_CAMERA_H
