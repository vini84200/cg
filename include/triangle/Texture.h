#ifndef TRIANGLE_TEXTURE_H
#define TRIANGLE_TEXTURE_H

#include "RenderTarget.h"
#include "imgui.h"
#include <cassert>
#include <memory>
#include <string>
#include <vector>

enum class TextureSampler { NearestNeighbour, Bilinear, Trilinear };

typedef std::vector<ColorPixel> TextureBuffer;

class Texture {
  public:
    Texture();
    Texture(const TextureBuffer &data, int width, int height);
    Texture(std::string path);
    Texture(Texture &&)                 = default;
    Texture(const Texture &)            = default;
    Texture &operator=(Texture &&)      = default;
    Texture &operator=(const Texture &) = default;
    ~Texture();

  private:
    std::shared_ptr<TextureBuffer> textureData;
    int mipLevels = 0;
    int height, width;
    TextureSampler textureSamplerType
        = TextureSampler::NearestNeighbour;
    std::vector<int> mipmapOffset;
    std::vector<int> mipmapWidths;
    GLuint textureIndex;

  public:
    inline ColorPixel getTexel(int x, int y, int level) const {
        const int pos = getPos(x, y, level);
        assert(pos < textureData->size());
        return textureData->at(pos);
    }
    inline int getHeight() { return height; }
    inline int getWidth() { return width; }
    inline int getMipLevels() { return mipLevels; }
    inline TextureSampler getTextureSampler() {
        return textureSamplerType;
    }
    inline void setTextureSampler(TextureSampler ts) {
        textureSamplerType = ts;
    }
    inline GLuint getTextureIndex() { return textureIndex; }
    inline ImTextureID getTextureId() {
        return (ImTextureID)textureIndex;
    }
    void generateMipMaps(int levels);
    ColorPixel samplePoint(float s, float t, float ds,
                           float dt) const;

  private:
    // Private methods
    ColorPixel samplePointNearestNeighbour(float s, float t) const;
    ColorPixel samplePointBilinear(float s, float t) const;
    ColorPixel samplePointTrilinear(float s, float t, float ds,
                                    float dt) const;

    // Inline
  private:
    inline int getBaseSize() { return width * height; }
    inline int getTotalSize() { return 2 * getBaseSize(); }
    inline int getPos(int x, int y, int level) const {
        return getLevelOffset(level) + mipmapWidths.at(level) * y + x;
    }
    inline int getLevelOffset(int level) const {
        assert(level < mipLevels && level >= 0);
        return mipmapOffset.at(level);
    }
    inline int getLevelWidht(int level) {
        if (level == 0) {
            return width;
        } else {
            return (getLevelWidht(level - 1) / 2);
        }
    }
    inline int getLevelHeight(int level) {
        if (level == 0) {
            return height;
        } else {
            return getLevelHeight(level - 1) / 2;
        }
    }
    inline int getLevelSize(int level) {
        return getLevelWidht(level) * getLevelHeight(level);
    }
};

#endif // TRIANGLE_TEXTURE_H