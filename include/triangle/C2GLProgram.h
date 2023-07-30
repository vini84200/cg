//
// Created by vini84200 on 6/29/23.
//

#ifndef TRIANGLE_C2GLPROGRAM_H
#define TRIANGLE_C2GLPROGRAM_H

#include "Material.h"
#include "Object.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <cwchar>

struct FragVertex {
    glm::vec4 position;
    glm::vec4 positionWorld;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec4 color;

    float wInv;

    static FragVertex interpolate(FragVertex v1, FragVertex v2, float t) {
        return FragVertex(
            v1.position * (1 - t) + v2.position * t,
            v1.positionWorld * (1 - t) + v2.positionWorld * t,
            v1.normal * (1 - t) + v2.normal * t, v1.uv * (1 - t) + v2.uv * t,
            v1.color * (1 - t) + v2.color * t, v1.wInv * (1 - t) + v2.wInv * t);
    }

    FragVertex() = default;

    FragVertex(glm::vec4 position, glm::vec4 positionWorld, glm::vec3 normal,
               glm::vec2 uv, glm::vec4 color)
        : position(position), normal(normal), uv(uv), wInv{1.0}, color(color),
          positionWorld(positionWorld){};

    FragVertex(glm::vec4 position, glm::vec4 positionWorld, glm::vec3 normal,
               glm::vec2 uv, glm::vec4 color, float wInv)
        : position(position), normal(normal), uv(uv), wInv{wInv}, color(color),
          positionWorld(positionWorld){};

    void perpectiveDivide() {
        float w = position.w;
        position /= w;
        positionWorld /= w;
        normal /= w;
        uv /= w;
        wInv = 1.0f / w;
    }

    void finish() {
        normal /= wInv;
        uv /= wInv;
        positionWorld /= wInv;
    }
    static FragVertex getDeltaY(FragVertex &top, FragVertex &bot) {
        float dy = top.position.y - bot.position.y;
        return FragVertex((top.position - bot.position) / dy,
                          (top.positionWorld - bot.positionWorld) / dy,
                          (top.normal - bot.normal) / dy,
                          (top.uv - bot.uv) / dy, (top.color - bot.color) / dy,
                          (top.wInv - bot.wInv) / dy);
    }
    static FragVertex add(FragVertex a, FragVertex b) {
        return FragVertex(a.position + b.position,
                          a.positionWorld + b.positionWorld,
                          a.normal + b.normal, a.uv + b.uv, a.color + b.color,
                          a.wInv + b.wInv);
    }
    FragVertex operator*(const float t) const {
        return FragVertex(position * t, positionWorld * t, normal * t, uv * t,
                          color * t, wInv * t);
    }
    FragVertex operator+(const FragVertex other) const {
        return FragVertex::add(*this, other);
    }
    static FragVertex getDeltaX(const FragVertex &left,
                                const FragVertex &right) {
        float dx = right.position.x - left.position.x;
        return FragVertex(
            (right.position - left.position) / dx,
            (right.positionWorld - left.positionWorld) / dx,
            (right.normal - left.normal) / dx, (right.uv - left.uv) / dx,
            (right.color - left.color) / dx, (right.wInv - left.wInv) / dx);
    }
};

struct Pixel {
    glm::vec3 color;
    float depth;
};

class C2GLProgram {

  public:
    const glm::mat4 &getModelMatrix() const;
    void setModelMatrix(const glm::mat4 &modelMatrix);
    const glm::mat4 &getViewMatrix() const;
    void setViewMatrix(const glm::mat4 &viewMatrix);
    const glm::mat4 &getProjectionMatrix() const;
    void setProjectionMatrix(const glm::mat4 &projectionMatrix);
    const glm::mat3 &getNormalMatrix() const;
    const Material &getMaterial() const;
    void setMaterial(const Material &material);

  private:
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    Material material;

  protected:
    glm::mat3 normalMatrix;

  public:
    virtual FragVertex vertexShader(const Vertex &vertex) const = 0;

    virtual Pixel fragmentShader(const FragVertex &vertex,
                                 const FragVertex &dFdx,
                                 const FragVertex &dFdy) const = 0;

    const glm::vec4 &getViewPos() const;

    void setViewPos(const glm::vec4 &viewPos);

    glm::vec4 view_pos;

    virtual void updateNormalMatrix();
};

#endif // TRIANGLE_C2GLPROGRAM_H
