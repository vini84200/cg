//
// Created by vini84200 on 6/1/23.
//

#include "TriangleObject.h"
#include "Material.h"
#include "ObjectFromFileIn.h"
#include "Window.h"
#include "imgui.h"
#include <vector>

TriangleObject::TriangleObject()
          : material_(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1),
                      glm::vec3(1, 1, 1), 1) {

    setIndecesType(TriangleType);
    clearVertices();
    addVertex(Vertex(glm::vec3(-0.90f, -0.90f, 0.f),
                     glm::vec3(1, 0, 0), glm::vec2(0, 0)));
    addVertex(Vertex(glm::vec3(0.90f, -0.90f, 0.f),
                     glm::vec3(0, 1, 0), glm::vec2(1, 0)));
    addVertex(Vertex(glm::vec3(0.f, 0.85f, 0.f), glm::vec3(0, 0, 1),
                     glm::vec2(0.5, 1)));

    initVAO();

    printf("Initialized\n");
}

void TriangleObject::renderImGui() {}

std::string TriangleObject::getName() const { return "Triangle"; }

void TriangleObject::update(float dt) {}

std::vector<CallSpan> TriangleObject::getCallSpan() {
    std::vector<CallSpan> callSpans;
    callSpans.emplace_back(0, 3, 0);
    return callSpans;
}

Material *TriangleObject::getMaterial(int index) {
    return &material_;
}
