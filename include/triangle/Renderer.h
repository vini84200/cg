//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_RENDERER_H
#define TRIANGLE_RENDERER_H


#include <string>
#include "Camera.h"
#include "Object.h"
#include "Scene.h"

/**
 * This is the base class for all renderers.
 * It defines the interface that all renderers must implement.
 */
class Renderer {
public:
    virtual void render(
            Scene *scene,
            Camera *camera) = 0;
};


#endif //TRIANGLE_RENDERER_H
