//
// Created by vini84200 on 6/29/23.
//

#include "C2GLProgram.h"
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

FragVertex C2GLProgram::vertexShader(const Vertex &vertex) const {
    return {
            getProjectionMatrix() * getViewMatrix() * getModelMatrix() * vertex.position,
            glm::normalize(getNormalMatrix() * vertex.normal),
            vertex.texture,
            glm::vec4(0.0, 0.0, 0.0, 0.0)
    };
}

Pixel C2GLProgram::fragmentShader(FragVertex &vertex) {
    // Phong shading
    glm::vec3 lightDir = glm::normalize(glm::vec3 (0.0, 0.0, 1.0));

    float diffuse = glm::max(glm::dot(vertex.normal, lightDir), 0.0f);
    glm::vec3 diffuseColor = material.diffuse * diffuse;
    float ambient = 1;
    glm::vec3 ambientColor = material.ambient * ambient;
    float energyConservation = (material.shine + 8.0f) / (8.0f * M_PI);
    glm::vec3 viewDir = glm::normalize(view_pos - vertex.position);
    glm::vec3 halfVector = glm::normalize(lightDir + viewDir);
    float specular = glm::pow(glm::max(glm::dot(vertex.normal, halfVector), 0.0f), material.shine) * energyConservation;
    glm::vec3 specularColor = material.specular * specular;
    glm::vec3 color = glm::clamp(diffuseColor + ambientColor + specularColor, 0.0f, 1.0f);


    return {color, vertex.position.z};
}

const glm::mat4 &C2GLProgram::getModelMatrix() const {
    return modelMatrix;
}

void C2GLProgram::setModelMatrix(const glm::mat4 &modelMatrix) {
    normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
    C2GLProgram::modelMatrix = modelMatrix;
}

const glm::mat4 &C2GLProgram::getViewMatrix() const {
    return viewMatrix;
}

void C2GLProgram::setViewMatrix(const glm::mat4 &viewMatrix) {
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

const glm::vec4 &C2GLProgram::getViewPos() const {
    return view_pos;
}

void C2GLProgram::setViewPos(const glm::vec4 &viewPos) {
    view_pos = viewPos;
}
