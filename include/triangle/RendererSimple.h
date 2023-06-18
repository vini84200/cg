//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_RENDERERSIMPLE_H
#define TRIANGLE_RENDERERSIMPLE_H


#include "Camera.h"
#include "Renderer.h"
#include <memory>
#include <vector>
#include <array>


class RendererSimple : public Renderer {
public:
    RendererSimple();

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

    enum ShaderSubroutine {
        NO_SHADE,
        GOURAD_AD,
        GOURAD_SPEC,
        PHONG,
        NUM_SUBROUTINES
    };
    ShaderSubroutine shaderSubroutine = GOURAD_AD;
    std::array<GLuint, NUM_SUBROUTINES> shaderSubroutineIndicesVS;
    std::array<GLuint, NUM_SUBROUTINES> shaderSubroutineIndicesFS;


    void renderObject(std::shared_ptr<Object> object);

    bool backFaceCulling_ = true;
    bool ccw_ = false;
    bool colorOverride_ = false;
    glm::vec3 newColor = glm::vec3(1, 1, 1);

    GLint lightModelVSLocation;
    GLint lightModelFSLocation;
};


#endif //TRIANGLE_RENDERERSIMPLE_H
