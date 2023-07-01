//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_RENDERTARGET_H
#define TRIANGLE_RENDERTARGET_H

#include <limits>
#include <vector>
#include "glad/glad.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"


struct ColorPixel {
    ColorPixel(glm::vec3 cor) {
        r = cor.r * 255;
        g = cor.g * 255;
        b = cor.b * 255;
        a = 255;
    }

    unsigned char r, g, b, a;
    ColorPixel() = default;
    ColorPixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
    ColorPixel(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b), a(255) {}
};

struct DepthPixel {
    float depth;
    DepthPixel() : depth(std::numeric_limits<float>::min()) {}
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
    std::vector<ColorPixel> pixels;
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

    inline void setPixel(int x, int y, ColorPixel pixel, DepthPixel depth) {
        assert(isInside(x, y));
        int index = getPixelIndex(x, y);
        pixels[index] = pixel;
        depthPixels[index] = depth;
    }

    inline void checkAndSetPixel(int x, int y, ColorPixel pixel, DepthPixel depth) {
        if (!isInside(x, y)) return;
        int index = getPixelIndex(x, y);
        if (depthPixels[index].depth < depth.depth) {
            pixels[index] = pixel;
            depthPixels[index] = depth;
        }
    }

    void render();

    void Clear(ColorPixel pixel) {
        pixels.assign(pixels.size(), pixel);
        depthPixels.assign(depthPixels.size(), DepthPixel());
    }


    glm::mat4 getViewportMatrix() const;

    void renderImGui();
};


#endif //TRIANGLE_RENDERTARGET_H
