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

    void render() override;

private:

    std::shared_ptr<Camera> camera;
    std::vector<Object>* objects;
    std::vector<void*>* shaders;
    
};


#endif //TRIANGLE_RENDERERSIMPLE_H
