//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_RENDERTARGET_H
#define TRIANGLE_RENDERTARGET_H

#include <vector>
#include "glad/glad.h"
#include "glm/fwd.hpp"


struct Pixel {
    unsigned char r, g, b, a;
    Pixel() = default;
    Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
    Pixel(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b), a(255) {}
};

struct DepthPixel {
    float depth;
    DepthPixel() : depth(-1.0f) {}
    DepthPixel(float depth) : depth(depth) {}
};

struct RenderTargetConfig {
    int width;
    int height;
    int capacity;

    inline int requiredCapacity() const {
        return width * height;
    }
};

class RenderTarget {
    std::vector<Pixel> pixels;
    std::vector<DepthPixel> depthPixels;
    RenderTargetConfig currentConfig;

    inline int isInside(int x, int y) {
        return x >= 0 && x < currentConfig.width && y >= 0 && y < currentConfig.height;
    }

    enum VAO_IDs_RT {
        ModelSpace, NumVAOs
    };

    enum Buffer_IDs_RT {
        PositionBuffer, TextureCoordBuffer, NumBuffers
    };

    enum Attrib_IDs_RT {
        vPosition = 0,  vTextureCoord = 1
    };

    GLuint program;
    GLuint vao_[NumVAOs];
    GLuint vbo_[NumBuffers];
    GLuint texture;
public:
    RenderTarget();
    void init(RenderTargetConfig config);
    void activate();
    void deactivate();
    void onResize(int width, int height);

    inline int getPixelIndex(int x, int y) const {
        return y * currentConfig.width + x;
    }

    inline bool depthTest(int x, int y, float depth) {
        if (!isInside(x, y)) return false;
        int index = getPixelIndex(x, y);
        return depthPixels[index].depth > depth;
    }

    inline void setPixel(int x, int y, Pixel pixel, DepthPixel depth) {
        assert(isInside(x, y));
        int index = getPixelIndex(x, y);
        pixels[index] = pixel;
        depthPixels[index] = depth;
    }

    inline void checkAndSetPixel(int x, int y, Pixel pixel, DepthPixel depth) {
        if (!isInside(x, y)) return;
        int index = getPixelIndex(x, y);
        if (depthPixels[index].depth < depth.depth) {
            pixels[index] = pixel;
            depthPixels[index] = depth;
        }
    }

    void render();

    void Clear(Pixel pixel) {
        pixels.assign(pixels.size(), pixel);
        depthPixels.assign(depthPixels.size(), DepthPixel());
    }


    glm::mat4 getViewportMatrix() const;

    void renderImGui();
};


#endif //TRIANGLE_RENDERTARGET_H
