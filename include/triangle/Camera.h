//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_CAMERA_H
#define TRIANGLE_CAMERA_H

#include "glm/glm.hpp"

class Camera {


    glm::mat4 getViewMatrix();

public:
    Camera();
    void renderImGui();

protected:
    glm::vec4 pos;
    glm::vec4 forward;

    float fov;
    float near;
    float far;
    float aspectRatio;


    glm::mat4 getProjectionMatrix();
};


#endif //TRIANGLE_CAMERA_H
