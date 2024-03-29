#include "Texture.h"
#include "C2GLProgram.h"
#include "RenderTarget.h"
#include <GL/gl.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <memory>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

Texture::Texture() {
    textureData = std::make_shared<TextureBuffer>();
    textureData->emplace_back();
    width        = 1;
    height       = 1;
    mipLevels    = 0;
    textureIndex = -1;
}

Texture::~Texture() {}

Texture::Texture(const TextureBuffer &data, int width, int height) {
    // Copy the data to the buffer
    textureData->assign(data.begin(), data.end());
    mipLevels = 0;
    mipmapOffset.emplace_back(0);
    mipmapWidths.emplace_back(width);
    this->width  = width;
    this->height = height;

    glGenTextures(1, &textureIndex);
    glBindTexture(GL_TEXTURE_2D, textureIndex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(std::string path) : path(path) {
    textureData  = std::make_shared<TextureBuffer>();
    int channels = -1;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data =
        stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (data) {
        for (int i = 0; i < width * height * 4; i += 4) {
            textureData->emplace_back(data[i], data[i + 1], data[i + 2],
                                      data[i + 3]);
        }
        mipLevels = 1;
        mipmapOffset.emplace_back(0);
        mipmapWidths.emplace_back(width);
        glGenTextures(1, &textureIndex);
        glBindTexture(GL_TEXTURE_2D, textureIndex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        int maxDimension = std::max(width, height);
        int mipLevels    = std::log2(maxDimension);
        printf("Mip levels: %d\n", mipLevels);

        generateMipMaps(mipLevels);

    } else {
        throw std::runtime_error("Failed to load the image texture: " + path);
    }
}

void Texture::generateMipMaps(int levels) {
    if (levels <= mipLevels) {
        // These miplevels are already loaded
        return;
    }
    for (int i = 0; i <= levels; i++) {
        if (mipLevels > i) {
            continue;
        }
        mipmapOffset.emplace_back(textureData->size());
        int levelWidth = getLevelWidht(i);
        mipmapWidths.emplace_back(levelWidth);
        int levelHeight = getLevelHeight(i);
        mipLevels++;
        for (int t = 0; t < levelHeight; t++) {
            for (int s = 0; s < levelWidth; s++) {
                // Calculate the color pixel of the 4 original pixels
                const ColorPixel a = getTexel(s * 2, t * 2, i - 1);
                const ColorPixel b = getTexel(s * 2 + 1, t * 2, i - 1);
                const ColorPixel c = getTexel(s * 2, t * 2 + 1, i - 1);
                const ColorPixel d = getTexel(s * 2 + 1, t * 2 + 1, i - 1);

                const ColorPixel ab           = ColorPixel::lerp(a, b, 0.5f);
                const ColorPixel cd           = ColorPixel::lerp(c, d, 0.5f);
                const ColorPixel interpolated = ColorPixel::lerp(ab, cd, 0.5f);
                textureData->emplace_back(interpolated);
            }
        }
    }
}

ColorPixel Texture::samplePoint(float s, float t, const glm::vec2 &dUVdx,
                                const glm::vec2 &dUVdy) const {
    s = std::clamp(s, 0.f, 1.f);
    t = std::clamp(t, 0.f, 1.f);
    switch (textureSamplerType) {
    case TextureSampler::NearestNeighbour:
        return samplePointNearestNeighbour(s, t);
    case TextureSampler::Bilinear:
        return samplePointBilinear(s, t);
    case TextureSampler::Trilinear:
        return samplePointTrilinear(s, t, dUVdx, dUVdy);
    }
}

ColorPixel Texture::samplePointNearestNeighbour(float s, float t) const {
    const int x = std::ceil(s * (width - 1));
    const int y = std::ceil(t * (height - 1));
    return getTexel(x, y, 0);
}

ColorPixel Texture::samplePointBilinear(float s, float t) const {
    // Get the 4 nearest points, and interpolate between them
    float x              = s * (width - 1);
    float y              = t * (height - 1);
    const ColorPixel a   = getTexel(std::floor(x), std::floor(y), 0);
    const ColorPixel b   = getTexel(std::ceil(x), std::floor(y), 0);
    const ColorPixel c   = getTexel(std::floor(x), std::ceil(y), 0);
    const ColorPixel d   = getTexel(std::ceil(x), std::ceil(y), 0);
    const ColorPixel ab  = ColorPixel::lerp(a, b, x - std::floor(x));
    const ColorPixel cd  = ColorPixel::lerp(c, d, x - std::floor(x));
    const ColorPixel res = ColorPixel::lerp(ab, cd, y - std::floor(y));
    return res;
}

ColorPixel Texture::samplePointTrilinear(float s, float t, glm::vec2 dUVdx,
                                         glm::vec2 dUVdy) const {
    // TODO
    float x = s * (width - 1);
    float y = t * (height - 1);

    float level = getLevel(dUVdx, dUVdy);

    const int lowLevel = std::floor(level);
    float lowX         = x / std::pow(2, lowLevel);
    float lowY         = y / std::pow(2, lowLevel);

    if (lowX >= getWidth())
        lowX = fmod(lowX, getWidth());
    if (lowY >= getHeight())
        lowY = fmod(lowY, getHeight());

    const ColorPixel a = getTexel(std::floor(lowX), std::floor(lowY), lowLevel);
    const ColorPixel b = getTexel(std::ceil(lowX), std::floor(lowY), lowLevel);
    const ColorPixel c = getTexel(std::floor(lowX), std::ceil(lowY), lowLevel);
    const ColorPixel d = getTexel(std::ceil(lowX), std::ceil(lowY), lowLevel);
    const ColorPixel ab  = ColorPixel::lerp(a, b, lowX - std::floor(lowX));
    const ColorPixel cd  = ColorPixel::lerp(c, d, lowX - std::floor(lowX));
    const ColorPixel low = ColorPixel::lerp(ab, cd, lowY - std::floor(lowY));

    const int highLevel = std::ceil(level);
    float highX         = x / std::pow(2, highLevel);
    float highY         = y / std::pow(2, highLevel);

    if (highX > getWidth() - 1)
        highX = fmod(highX, getWidth() - 1);
    if (highY > getHeight() - 1)
        highY = fmod(highY, getHeight() - 1);

    const ColorPixel A =
        getTexel(std::floor(highX), std::floor(highY), highLevel);
    const ColorPixel B =
        getTexel(std::ceil(highX), std::floor(highY), highLevel);
    const ColorPixel C =
        getTexel(std::floor(highX), std::ceil(highY), highLevel);
    const ColorPixel D =
        getTexel(std::ceil(highX), std::ceil(highY), highLevel);
    const ColorPixel AB   = ColorPixel::lerp(A, B, highX - std::floor(highX));
    const ColorPixel CD   = ColorPixel::lerp(C, D, highX - std::floor(highX));
    const ColorPixel high = ColorPixel::lerp(AB, CD, highY - std::floor(highY));

    const ColorPixel res = ColorPixel::lerp(low, high, level - lowLevel);
    return res;
}

std::string Texture::getTexturePath() { return path; }

bool Texture::hasMipmaps() { return mipLevels > 1; }

int Texture::getMipmaps() { return mipLevels; }

void Texture::setTextureSampler(TextureSampler ts) {
    textureSamplerType = ts;
    if (ts == TextureSampler::Trilinear) {
        glBindTexture(GL_TEXTURE_2D, getTextureIndex());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else if (ts == TextureSampler::Bilinear) {
        glBindTexture(GL_TEXTURE_2D, getTextureIndex());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else if (ts == TextureSampler::NearestNeighbour) {
        glBindTexture(GL_TEXTURE_2D, getTextureIndex());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
float Texture::getLevel(glm::vec2 dUVdx, glm::vec2 dUVdy) const {
    const glm::vec2 size       = glm::vec2(width - 1, height - 1);
    const glm::vec2 dUVdxSized = dUVdx * size;
    const glm::vec2 dUVdySized = dUVdy * size;

    //    float dmaxsqr =
    //        std::max(glm::dot(dUVdxSized, dUVdxSized), glm::dot(dUVdySized,
    //        dUVdySized));
    float dmaxsqr =
        glm::dot(dUVdxSized,
                 dUVdxSized); // Use only dUVdx for now, since dUVdy is broken
    float rawLevel = std::max(0.0f, 0.5f * std::log2(dmaxsqr));

    if (rawLevel > mipLevels - 1)
        return mipLevels - 1;

    if (isnan(rawLevel) || isinf(rawLevel)) {
        printf("The level is nan or inf\n");
        return 0;
    }

    return rawLevel;
}
