//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_CAMERA_H
#define TRIANGLE_CAMERA_H

#include "glm/glm.hpp"

class Camera {
    Camera();

    protected:

    glm::vec4 pos;
    glm::vec4 forward;
    
};


#endif //TRIANGLE_CAMERA_H
