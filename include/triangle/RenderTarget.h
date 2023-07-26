//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_RENDERTARGET_H
#define TRIANGLE_RENDERTARGET_H

#include "glad/glad.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <limits>
#include <vector>


struct ColorPixel {
    ColorPixel(glm::vec3 cor) {
        r = cor.r * 255;
        g = cor.g * 255;
        b = cor.b * 255;
        a = 255;
    }

    unsigned char r, g, b, a;
    ColorPixel() = default;
    ColorPixel(unsigned char r, unsigned char g, unsigned char b,
               unsigned char a)
              : r(r), g(g), b(b), a(a) {}
    ColorPixel(unsigned char r, unsigned char g, unsigned char b)
              : r(r), g(g), b(b), a(255) {}
    inline static ColorPixel lerp(const ColorPixel &a,
                                  const ColorPixel &b, float t) {
        return {(unsigned char)(b.r * t + a.r * (1 - t)),
                (unsigned char)(b.g * t + a.g * (1 - t)),
                (unsigned char)(b.b * t + a.b * (1 - t)),
                (unsigned char)(b.a * t + a.a * (1 - t))};
    }
    glm::vec3 toVec3() const {
        return {
            ((float)r) / 255.f,
            ((float)g) / 255.f,
            ((float)b) / 255.f,
        };
    }
    glm::vec4 toVec4() const {
        return {
            ((float)r) / 255.f,
            ((float)g) / 255.f,
            ((float)b) / 255.f,
            ((float)a) / 255.f,
        };
    }
};

struct DepthPixel {
    float depth;
    DepthPixel() : depth(std::numeric_limits<float>::max()) {}
    DepthPixel(float depth) : depth(depth) {}

    DepthPixel operator-(float depth) const {
        return {this->depth - depth};
    }

    DepthPixel operator-=(float depth) {
        this->depth -= depth;
        return *this;
    }

    DepthPixel operator-(DepthPixel depth) const {
        return {this->depth - depth.depth};
    }

    DepthPixel operator-=(DepthPixel depth) {
        this->depth -= depth.depth;
        return *this;
    }

    const float operator/(float depth) const {
        return this->depth / depth;
    }
};

struct RenderTargetConfig {
    int width;
    int height;
    int capacity;

    inline int requiredCapacity() const { return width * height; }

    float depthBufferStart = -1.0;
    float depthBufferEnd   = 1.0;
};

class RenderTarget {
    std::vector<ColorPixel> pixels;

  public:
    ColorPixel *getPixelData() const;

  private:
    std::vector<DepthPixel> depthPixels;
    RenderTargetConfig currentConfig;

    inline int isInside(int x, int y) {
        return x >= 0 && x < currentConfig.width && y >= 0
               && y < currentConfig.height;
    }

    enum VAO_IDs_RT { ModelSpace, NumVAOs };

    enum Buffer_IDs_RT {
        PositionBuffer,
        TextureCoordBuffer,
        NumBuffers
    };

    enum Attrib_IDs_RT { vPosition = 0, vTextureCoord = 1 };

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

    int getWidth() const;
    int getHeight() const;

    inline int getPixelIndex(int x, int y) const {
        return y * currentConfig.width + x;
    }

    inline bool depthTest(int x, int y, float depth) {
        if (!isInside(x, y))
            return false;
        int index = getPixelIndex(x, y);
        return depthPixels[index].depth > depth;
    }

    inline void setPixel(int x, int y, ColorPixel pixel,
                         DepthPixel depth) {
        assert(isInside(x, y));
        int index          = getPixelIndex(x, y);
        pixels[index]      = pixel;
        depthPixels[index] = depth;
    }

    inline void checkAndSetPixel(int x, int y, ColorPixel pixel,
                                 DepthPixel depth) {
        if (!isInside(x, y))
            return;
        int index = getPixelIndex(x, y);
        if (depthPixels[index].depth > depth.depth) {
            pixels[index]      = pixel;
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

    std::vector<DepthPixel> getDepthBuffer();

    GLuint depth_texture;
};


#endif // TRIANGLE_RENDERTARGET_H
