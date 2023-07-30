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

    std::string getTexturePath();

    bool hasMipmaps();

    int getMipmaps();

    float getLevel(glm::vec2 dUVdx, glm::vec2 dUVdy) const;

  private:
    std::shared_ptr<TextureBuffer> textureData;
    int mipLevels = 0;
    int height, width;
    TextureSampler textureSamplerType = TextureSampler::NearestNeighbour;
    std::vector<int> mipmapOffset;
    std::vector<int> mipmapWidths;
    GLuint textureIndex;
    std::string path = "NO PATH";

  public:
    inline ColorPixel getTexel(int x, int y, int level) const {
        const int pos = getPos(x, y, level);
       assert(pos < textureData->size());
        return textureData->at(pos);
    }
    inline int getHeight() const { return height; }
    inline int getWidth() const { return width; }
    inline int getMipLevels() const { return mipLevels; }
    inline TextureSampler getTextureSampler() const {
        return textureSamplerType;
    }
    void setTextureSampler(TextureSampler ts);
    inline GLuint getTextureIndex() { return textureIndex; }
    inline ImTextureID getTextureId() { return (ImTextureID)textureIndex; }
    void generateMipMaps(int levels);
    ColorPixel samplePoint(float s, float t, const glm::vec2 &dUVdx,
                           const glm::vec2 &dUVdy) const;

  private:
    // Private methods
    ColorPixel samplePointNearestNeighbour(float s, float t) const;
    ColorPixel samplePointBilinear(float s, float t) const;
    ColorPixel samplePointTrilinear(float s, float t, glm::vec2 dUVdx,
                                    glm::vec2 dUVdy) const;

    // Inline
  private:
    inline int getBaseSize() { return width * height; }
    inline int getTotalSize() { return 2 * getBaseSize(); }
    inline int getPos(int x, int y, int level) const {
        if (level >= mipLevels) {
            level = mipLevels - 1;
        }
        int x_ = x % getLevelWidht(level);
        int y_ = y % getLevelHeight(level);

        return getLevelOffset(level) + mipmapWidths.at(level) * y_ + x_;
    }
    inline int getLevelOffset(int level) const {
        assert(level < mipLevels && level >= 0);
        return mipmapOffset.at(level);
    }
    inline int getLevelWidht(int level) const {
        if (level == 0) {
            return width;
        } else {
            return (getLevelWidht(level - 1) / 2);
        }
    }
    inline int getLevelHeight(int level) const {
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
