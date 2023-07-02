//
// Created by vini84200 on 6/29/23.
//

#ifndef TRIANGLE_C2GLPROGRAM_H
#define TRIANGLE_C2GLPROGRAM_H

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "Object.h"
#include <cwchar>


struct FragVertex {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec4 color;

    float wInv;

    static FragVertex interpolate(FragVertex v1, FragVertex v2, float t) {
        return FragVertex(v1.position * (1 - t) + v2.position * t,
                          v1.normal * (1 - t) + v2.normal * t,
                          v1.uv * (1 - t) + v2.uv * t,
                          v1.color * (1 - t) + v2.color * t,
                          v1.wInv * (1 - t) + v2.wInv * t);
    }

    FragVertex() = default;

    FragVertex(glm::vec4 position, glm::vec3 normal, glm::vec2 uv, glm::vec4 color) : position(position),
                                                                                      normal(normal),
                                                                                      uv(uv), wInv{1.0},
                                                                                      color(color) {};

    FragVertex(glm::vec4 position, glm::vec3 normal, glm::vec2 uv, glm::vec4 color, float wInv) : position(position),
                                                                                                  normal(normal),
                                                                                                  uv(uv), wInv{wInv},
                                                                                                  color(color) {};

    void perpectiveDivide() {
        float w = position.w;
        position /= w;
        normal /= w;
        uv /= w;
        wInv = 1.0f / w;
    }

    void finish() {
        normal /= wInv;
        uv /= wInv;
    }

};

struct Pixel {
    glm::vec3 color;
    float depth;
};

class C2GLProgram {

private:
    glm::mat4 modelMatrix;
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
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat3 normalMatrix;

    Material material;

public:
    C2GLProgram() = default;

    FragVertex vertexShader(const Vertex &vertex) const;

    Pixel fragmentShader(FragVertex &vertex);

    const glm::vec4 &getViewPos() const;

    void setViewPos(const glm::vec4 &viewPos);

    glm::vec4 view_pos;
};


#endif //TRIANGLE_C2GLPROGRAM_H
