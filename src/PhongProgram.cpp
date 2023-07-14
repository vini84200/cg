#include "PhongProgram.h"

FragVertex PhongProgram::vertexShader(const Vertex &vertex) const {
    return {
            getProjectionMatrix() * getViewMatrix() * getModelMatrix() * vertex.position,
            getModelMatrix() * vertex.position,
            glm::normalize(getNormalMatrix() * vertex.normal),
            vertex.texture,
            glm::vec4(0.0, 0.0, 0.0, 0.0)
    };
}

Pixel PhongProgram::fragmentShader(FragVertex &vertex) {
    // Phong shading
    glm::vec3 lightDir = glm::normalize(glm::vec3 (0.0, 0.0, 1.0));
    glm::vec3 normal = glm::normalize(vertex.normal);
    const Material &material = getMaterial();

    float diffuse = glm::max(glm::dot(normal, lightDir), 0.0f);
    glm::vec3 diffuseColor = material.diffuse * diffuse;
    float ambient = 1;
    glm::vec3 ambientColor = material.ambient * ambient;
    float energyConservation = (material.shine + 8.0f) / (8.0f * M_PI);
    glm::vec3 viewDir = glm::normalize(view_pos - vertex.positionWorld);
    glm::vec3 halfVector = glm::normalize(lightDir + viewDir);
    float specular = glm::pow(glm::max(glm::dot(normal, halfVector), 0.0f), material.shine) * energyConservation;
    glm::vec3 specularColor = material.specular * specular;
    glm::vec3 color = glm::clamp(diffuseColor + ambientColor + specularColor, 0.0f, 1.0f);


    return {color, vertex.position.z};
}

