//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_RENDERER_H
#define TRIANGLE_RENDERER_H


#include "Camera.h"
#include "Object.h"
#include "Scene.h"
#include <string>

/**
 * This is the base class for all renderers.
 * It defines the interface that all renderers must implement.
 */
class Renderer {
  public:
    virtual void render(Scene *scene, Camera *camera) = 0;
    virtual void renderImGui()                        = 0;
    virtual void update(float dt)                     = 0;
    virtual std::string getName() const               = 0;

    virtual void start(){};
    virtual void pause(){};
    virtual void resume(){};

    virtual void onResize(int width, int height){};
};


#endif // TRIANGLE_RENDERER_H
