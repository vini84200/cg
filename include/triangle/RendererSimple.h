//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_RENDERERSIMPLE_H
#define TRIANGLE_RENDERERSIMPLE_H


#include "Camera.h"
#include "Renderer.h"
#include <memory>
#include <vector>

class RendererSimple : public Renderer {
public:
    RendererSimple();

    void render(Scene *scene, Camera *camera) override;

    void renderImGui() override;

private:

    std::shared_ptr<Camera> camera;
    GLuint program;

    void renderObject(std::shared_ptr<Object> object);
};


#endif //TRIANGLE_RENDERERSIMPLE_H
