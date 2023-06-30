//
// Created by vini84200 on 6/16/23.
//

#include "Rasterizer.h"
#include "RenderTarget.h"
#include "Renderer.h"

#ifndef TRIANGLE_RENDERERCLOSETOGLWITHRASTER_H
#define TRIANGLE_RENDERERCLOSETOGLWITHRASTER_H


class RendererCloseToGlWithRasterizer : public Renderer {
public:
    RendererCloseToGlWithRasterizer();

    void render(Scene *scene, Camera *camera) override;

    void renderImGui() override;

    void update(float dt) override;

    std::string getName() const override;

    void start() override;

    void pause() override;

    void resume() override;

    void onResize(int width, int height) override;

private:

    std::shared_ptr<Camera> camera;
    GLuint program{};
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
    RenderTarget renderTarget;
    Rasterizer rasterizer;
};


#endif //TRIANGLE_RENDERERCLOSETOGLWITHRASTER_H
