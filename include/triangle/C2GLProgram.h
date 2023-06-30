//
// Created by vini84200 on 6/29/23.
//

#ifndef TRIANGLE_C2GLPROGRAM_H
#define TRIANGLE_C2GLPROGRAM_H

#include "glm/glm.hpp"
#include "Object.h"


struct FragVertex {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec2 uv;

    float w;

    static FragVertex binomialInterpolation(FragVertex v1, FragVertex v2, float t) {
        return FragVertex(v1.position * (1 - t) + v2.position * t,
                      v1.normal * (1 - t) + v2.normal * t,
                      v1.uv * (1 - t) + v2.uv * t,
                      v1.w * (1 - t) + v2.w * t);
    }
    FragVertex() = default;
    FragVertex(glm::vec4 position, glm::vec3 normal, glm::vec2 uv) : position(position), normal(normal),
                                                                      uv(uv), w{1.0} {};
    FragVertex(glm::vec4 position, glm::vec3 normal, glm::vec2 uv, float w) : position(position), normal(normal),
                                                                        uv(uv), w(w) {};
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

    void setNormalMatrix(const glm::mat3 &normalMatrix);

    const Material &getMaterial() const;

    void setMaterial(const Material &material);

private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat3 normalMatrix;

    Material material;

public:
    C2GLProgram() = default;
    FragVertex vertexShader(const Vertex &vertex);
    Pixel fragmentShader(FragVertex &vertex);

};


#endif //TRIANGLE_C2GLPROGRAM_H
