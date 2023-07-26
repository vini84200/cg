//
// Created by vini84200 on 6/29/23.
//

#include "C2GLProgram.h"
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

const glm::mat4 &C2GLProgram::getModelMatrix() const {
    return modelMatrix;
}

void C2GLProgram::setModelMatrix(const glm::mat4 &modelMatrix) {
    C2GLProgram::modelMatrix = modelMatrix;
    updateNormalMatrix();
}

void C2GLProgram::updateNormalMatrix() {
    this->normalMatrix
        = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
}

const glm::mat4 &C2GLProgram::getViewMatrix() const {
    return viewMatrix;
}

void C2GLProgram::setViewMatrix(const glm::mat4 &viewMatrix) {
    C2GLProgram::viewMatrix = viewMatrix;
    updateNormalMatrix();
}

const glm::mat4 &C2GLProgram::getProjectionMatrix() const {
    return projectionMatrix;
}

void C2GLProgram::setProjectionMatrix(
    const glm::mat4 &projectionMatrix) {
    C2GLProgram::projectionMatrix = projectionMatrix;
}

const glm::mat3 &C2GLProgram::getNormalMatrix() const {
    return normalMatrix;
}


const Material &C2GLProgram::getMaterial() const { return material; }

void C2GLProgram::setMaterial(const Material &material) {
    C2GLProgram::material = material;
}

const glm::vec4 &C2GLProgram::getViewPos() const { return view_pos; }

void C2GLProgram::setViewPos(const glm::vec4 &viewPos) {
    view_pos = viewPos;
}
