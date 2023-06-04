//
// Created by vini84200 on 6/1/23.
//

#ifndef TRIANGLE_SCENE_H
#define TRIANGLE_SCENE_H


#include <memory>
#include <vector>
#include "Object.h"

class Scene {
public:
    Scene();

    void addObject( std::shared_ptr<Object> object );
    void removeObject( std::shared_ptr<Object> object );

    std::vector<std::shared_ptr<Object>> getObjects();
    void renderImGui();
    void update(float dt);

private:
    std::vector<std::shared_ptr<Object>> objects;
};


#endif //TRIANGLE_SCENE_H
