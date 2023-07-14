#include "GouradProgram.h"
#include "Object.h"
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <tracy/Tracy.hpp>

using namespace glm;

FragVertex GouradProgram::vertexShader(const Vertex &vertex) const {
    ZoneScoped;
    // Phong illumination model (Gourad shading)
    vec4 lightPInCameraSpace = vec4(2.0, 2.0, 1.0, 1.0);
    vec4 vertPosInCameraSpace = getViewMatrix() * getModelMatrix() * vertex.position;
    vec3 lightDir = normalize(lightPInCameraSpace - vertPosInCameraSpace);
    vec3 normal = normalize(getNormalMatrix() * vertex.normal);
    auto viewDir = vec3(0.0, 0.0, 1.0);
    const Material &material = getMaterial();
    float diffuse = max(dot(normal, lightDir), 0.f);
    float ambient = 1;
    float energyConservation = (material.shine + 8.0f) / (8.0f * M_PI);
    glm::vec3 halfVector = glm::normalize(lightDir + viewDir);
    float specular = glm::pow(glm::max(glm::dot(normal, halfVector), 0.0f), material.shine) * energyConservation;


    glm::vec3 color = diffuse * material.diffuse 
                                  + ambient * material.ambient
                                  + specular * material.specular;
    color = glm::clamp(color, 0.0f, 1.0f);

    return {
            getProjectionMatrix() * vertPosInCameraSpace,
            {0,0,0,0},
            normal,
            vertex.texture,
            glm::vec4(color, 1.0f)
    };
}

Pixel GouradProgram::fragmentShader(FragVertex &vertex) {
    ZoneScoped;

    return {vertex.color, vertex.position.z};
}

void GouradProgram::updateNormalMatrix() {
    this->normalMatrix = glm::transpose(glm::inverse(glm::mat3(getViewMatrix()* getModelMatrix())));
}

