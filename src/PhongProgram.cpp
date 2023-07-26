#include "PhongProgram.h"

FragVertex PhongProgram::vertexShader(const Vertex &vertex) const {
    return {getProjectionMatrix() * getViewMatrix() * getModelMatrix()
                * vertex.position,
            getModelMatrix() * vertex.position,
            glm::normalize(getNormalMatrix() * vertex.normal),
            vertex.texture, glm::vec4(0.0, 0.0, 0.0, 0.0)};
}

Pixel PhongProgram::fragmentShader(FragVertex &vertex) {
    // Phong shading
    // return {glm::vec3(vertex.uv, 0), vertex.position.z};
    glm::vec3 lightDir = glm::normalize(glm::vec3(1.0, 1.0, 1.0));
    glm::vec3 normal   = glm::normalize(vertex.normal);
    const Material &material = getMaterial();

    float diffuse = glm::max(glm::dot(normal, lightDir), 0.0f);
    glm::vec3 diffuseColor;
    glm::vec3 ambientColor;
    if (material.diffTexture.has_value()) {
        const Texture &tex = material.diffTexture.value();
        const ColorPixel cp
            = tex.samplePoint(vertex.uv.x, vertex.uv.y, 0.f, 0.f);
        diffuseColor = cp.toVec3() * diffuse;
        ambientColor = material.ambient * cp.toVec3();
    } else {
        diffuseColor = material.diffuse * diffuse;
        ambientColor = material.ambient;
    }
    float energyConservation
        = (material.shine + 8.0f) / (8.0f * M_PI);
    glm::vec3 viewDir
        = glm::normalize(view_pos - vertex.positionWorld);
    glm::vec3 halfVector = glm::normalize(lightDir + viewDir);
    float specular
        = glm::pow(glm::max(glm::dot(normal, halfVector), 0.0f),
                   material.shine)
          * energyConservation;
    glm::vec3 specularColor = material.specular * specular;
    glm::vec3 color         = glm::clamp(
        diffuseColor + ambientColor + specularColor, 0.0f, 1.0f);


    return {color, vertex.position.z};
}
