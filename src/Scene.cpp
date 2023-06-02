//
// Created by vini84200 on 6/1/23.
//

#include <stdexcept>
#include "Scene.h"
#include "imgui.h"

Scene::Scene() {

}

void Scene::addObject(std::shared_ptr<Object> object) {
    objects.push_back( object );
}

void Scene::removeObject(std::shared_ptr<Object> object) {
//    objects.erase( std::remove( objects.begin(), objects.end(), object ), objects.end() );
    throw std::runtime_error( "Not implemented" );
}

std::vector<std::shared_ptr<Object>> Scene::getObjects() {
    return objects;
}

void Scene::renderImGui() {
    ImGui::Begin( "Scene" );
    ImGui::Text( "Objects: %d", objects.size() );
    if (ImGui::CollapsingHeader("Objects")) {
        for (auto &object : objects) {
            if (ImGui::TreeNode(object->getName().c_str())) {
                object->renderImGui();
                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
}
