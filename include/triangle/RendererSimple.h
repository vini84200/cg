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

    void update(float dt) override;

private:

    std::shared_ptr<Camera> camera;
    GLuint program;
    enum RenderType {
        TRIANGLES,
        LINES,
        POINTS
    };
    RenderType renderType = TRIANGLES;

    void renderObject(std::shared_ptr<Object> object);

    bool backFaceCulling_ = true;
    bool ccw_;
};


#endif //TRIANGLE_RENDERERSIMPLE_H
