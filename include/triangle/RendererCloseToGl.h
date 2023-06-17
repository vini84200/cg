//
// Created by vini84200 on 6/16/23.
//

#ifndef TRIANGLE_RENDERERCLOSETOGL_H
#define TRIANGLE_RENDERERCLOSETOGL_H


#include "Renderer.h"

class RendererCloseToGl : public Renderer {
public:
    RendererCloseToGl();

    void render(Scene *scene, Camera *camera) override;

    void renderImGui() override;

    void update(float dt) override;

    std::string getName() const override;

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
    bool ccw_ = false;
    bool colorOverride_ = false;
    glm::vec3 newColor = glm::vec3(1, 1, 1);
};


#endif //TRIANGLE_RENDERERCLOSETOGL_H
