//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_RENDERER_H
#define TRIANGLE_RENDERER_H


#include <string>
#include "Camera.h"
#include "Object.h"

/**
 * This is the base class for all renderers.
 * It defines the interface that all renderers must implement.
 */
class Renderer {
    virtual std::string getRendererName() = 0;
    virtual void render() = 0;
    virtual void setCamera( Camera* camera ) = 0;
    virtual void setObjects( Object* objects ) = 0;
    virtual void setShaders( void* shaders ) = 0;

    virtual int getRenderTargetsCount() = 0;

    virtual void setRenderTargets( void* renderTargets ) = 0;
};


#endif //TRIANGLE_RENDERER_H
