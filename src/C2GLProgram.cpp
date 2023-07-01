//
// Created by vini84200 on 6/29/23.
//

#include "C2GLProgram.h"

FragVertex C2GLProgram::vertexShader(const Vertex &vertex) const {
    return {
            getProjectionMatrix() * getViewMatrix() * getModelMatrix() * vertex.position,
            getNormalMatrix() * vertex.normal,
            vertex.texture,
            glm::vec4(0.0, 0.0, 0.0, 0.0)
            };
}

Pixel C2GLProgram::fragmentShader(FragVertex &vertex) {
    return {vertex.normal, vertex.position.w};
}

const glm::mat4 &C2GLProgram::getModelMatrix() const {
    return modelMatrix;
}

void C2GLProgram::setModelMatrix(const glm::mat4 &modelMatrix) {
    normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
    C2GLProgram::modelMatrix = modelMatrix;
}

const glm::mat4 &C2GLProgram::getViewMatrix() const {
    return viewMatrix;
}

void C2GLProgram::setViewMatrix(const glm::mat4 &viewMatrix) {
    normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
    C2GLProgram::viewMatrix = viewMatrix;
}

const glm::mat4 &C2GLProgram::getProjectionMatrix() const {
    return projectionMatrix;
}

void C2GLProgram::setProjectionMatrix(const glm::mat4 &projectionMatrix) {
    C2GLProgram::projectionMatrix = projectionMatrix;
}

const glm::mat3 &C2GLProgram::getNormalMatrix() const {
    return normalMatrix;
}


const Material &C2GLProgram::getMaterial() const {
    return material;
}

void C2GLProgram::setMaterial(const Material &material) {
    C2GLProgram::material = material;
}
