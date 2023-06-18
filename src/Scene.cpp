//
// Created by vini84200 on 6/1/23.
//

#include <cstdio>
#include <stdexcept>
#include "Scene.h"
#include "imgui.h"
#include "nfd.h"
#include "ObjectFromFileIn.h"

Scene::Scene() {
    NFD_Init();
}

void Scene::addObject(std::shared_ptr<Object> object) {
    objects.push_back(object);
}

void Scene::removeObject(std::shared_ptr<Object> object) {
//    objects.erase( std::remove( objects.begin(), objects.end(), object ), objects.end() );
    throw std::runtime_error("Not implemented");
}

std::vector<std::shared_ptr<Object>> Scene::getObjects() {
    return objects;
}

void Scene::renderImGui() {
    ImGui::Begin("Scene");
    ImGui::Text("Objects: %d", objects.size());
    if (ImGui::CollapsingHeader("Objects")) {

        if (ImGui::Button("Add Object")) {
            // Open file dialog
            nfdchar_t *outPath = nullptr;
            nfdfilteritem_t filterItem[3] = {{"OBJ Files", "obj"},
                                             {"Custom 3D", "in"},
                                             {"All files", "*"}};
            nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 3, nullptr);

            if (result == NFD_OKAY) {
                std::string path(outPath);
                printf("Path: %s\n", path.c_str());
                // Get file extension
                std::string extension = path.substr(path.find_last_of(".") + 1);
                printf("Extension: %s\n", extension.c_str());
                if (extension == "obj") {
//                    std::shared_ptr<Object> object = std::make_shared<ObjectFromFileIn>(path);
//                    addObject(object);
            } else if (extension == "in") {
                std::shared_ptr<Object> object = std::make_shared<ObjectFromFileIn>(path);
                addObject(object);
            } else {
                throw std::runtime_error("Invalid file extension");
            }
            NFD_FreePath(outPath);
        } else if (result == NFD_CANCEL) {
            // Do nothing
        } else {
            throw std::runtime_error("Error opening file dialog");
        }
    }

        for (auto &object : objects) {
            if (ImGui::TreeNode(object->getName().c_str())) {
                object->renderImGui();
                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
}

void Scene::update(float dt) {
    for (auto &object: objects) {
        object->update(dt);
    }
}
