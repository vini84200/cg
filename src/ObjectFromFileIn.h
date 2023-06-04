//
// Created by vini84200 on 6/2/23.
//

#ifndef TRIANGLE_OBJECTFROMFILEIN_H
#define TRIANGLE_OBJECTFROMFILEIN_H


#include <vector>
#include "Object.h"

#define MAX_MATERIAL_COUNT 2

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shine;
};

class ObjectFromFileIn : public Object {
public:
    ObjectFromFileIn(std::string path);
    void renderImGui() override;
    std::string getName() const override;

private:
    std::string path;
    std::string name;

    std::vector<Material> materials;

    int material_count = 0;
public:
    void update(float dt) override;
};


#endif //TRIANGLE_OBJECTFROMFILEIN_H
