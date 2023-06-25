//
// Created by vini84200 on 6/23/23.
//

#include "Rasterizer.h"
#include "Object.h"
#include "RenderTarget.h"
#include "glm/geometric.hpp"
#include "glm/gtc/epsilon.hpp"
#include <cmath>
#include <vector>

void Rasterizer::preSortTriangleVertices(std::vector<Vertex> &vertices) {
    int numTriangles = vertices.size() / 3;
    for (int tri = 0; tri < numTriangles; tri++) {
        Vertex &v1 = vertices[tri * 3];
        Vertex &v2 = vertices[tri * 3 + 1];
        Vertex &v3 = vertices[tri * 3 + 2];
        // Sort the vertices by y. v1 is the top vertex with the lowest y
        // v2 is the middle vertex with the middle y
        // v3 is the bottom vertex with the highest y

        if (v2.position.y < v1.position.y) {
            std::swap(v1, v2);
        }
        if (v3.position.y < v2.position.y) {
            std::swap(v2, v3);
        }
        if (v2.position.y < v1.position.y) {
            std::swap(v1, v2);
        }


    }
}

void Rasterizer::renderObject(Object *object) {
    std::vector<glm::vec3> windingOrder;
    assert(object->cameraVertices.size() % 3 == 0);
    windingOrder.assign(object->cameraVertices.size()/3, glm::vec3(0, 0, 0));
    calculateWindingOrder(object->cameraVertices, windingOrder);
    preSortTriangleVertices(object->cameraVertices);
    int numTriangles = object->cameraVertices.size() / 3;
    for (int tri = 0; tri < numTriangles; tri++) {
        bool isFrontFacing = windingOrder[tri].z > 0;
        if (!isFrontFacing) {
            continue;
        }
        Vertex &v1 = object->cameraVertices[tri * 3];
        Vertex &v2 = object->cameraVertices[tri * 3 + 1];
        Vertex &v3 = object->cameraVertices[tri * 3 + 2];

        drawTriangle(v1, v2, v3);
    }
}

void Rasterizer::drawTriangle(Vertex &top, Vertex &mid, Vertex &bot) {

    assert(top.position.y <= mid.position.y);
    assert(mid.position.y <= bot.position.y);

    if (top.position.y == mid.position.y) {
        // Flat top triangle
        if (mid.position.x < top.position.x) {
            std::swap(top, mid);
        }
        drawFlatTopTriangle(top, mid, bot);
    } else if (mid.position.y == bot.position.y) {
        // Flat bottom triangle
        if (bot.position.x < mid.position.x) {
            std::swap(bot, mid);
        }
        drawFlatBottomTriangle(top, mid, bot);
    } else {
        // General triangle
        // Split the triangle into a flat bottom and flat top triangle

        // Check if the triangle is a left or right triangle
        // using edge equations
        glm::vec2 a_to_b = glm::normalize(top.position - bot.position);
        glm::vec2 w = glm::vec2(a_to_b.y, -a_to_b.x);
        float edge = glm::dot(w, glm::vec2(mid.position - bot.position));

        if (edge > 0) {
            // Left triangle
            Vertex intersection = interpolateVertex(top, bot, mid.position.y);
            drawFlatBottomTriangle(top, mid, intersection);
            drawFlatTopTriangle(mid, intersection, bot);
        } else if (edge < 0) {
            // Right triangle
            Vertex intersection = interpolateVertex(top, bot, mid.position.y);
            drawFlatBottomTriangle(top, intersection, mid);
            drawFlatTopTriangle(intersection, mid, bot);
        } else {
            // Degenerate triangle
            // It's a line or a point
            // TODO: Decide if there is a need to draw lines or points
            if (top.position.x == mid.position.x) {
                // It's a point
            } else {
                // It's a line
            }
        }


    }

}

void Rasterizer::drawFlatTopTriangle(Vertex &topL, Vertex &topR, Vertex &bot) {
    float deltaY = bot.position.y - topL.position.y;
    if (deltaY == 0) {
        // Degenerate triangle
        // It's a line or a point
        return;
    }

    int initialY = std::ceil(topL.position.y);
    int finalY = std::ceil(bot.position.y);
    for (int y = initialY; y < finalY; y++) {
        Vertex left = interpolateVertex(topL, bot, y);
        Vertex right = interpolateVertex(topR, bot, y);
        scanLine(left, right, y);
    }

}

void Rasterizer::drawFlatBottomTriangle(Vertex &top, Vertex &botL, Vertex &botR) {
    float deltaY = botL.position.y - top.position.y;
    if (deltaY == 0) {
        // Degenerate triangle
        // It's a line or a point
        return;
    }

    int initialY = std::ceil(top.position.y);
    int finalY =  std::ceil(botL.position.y);
    for (int y = initialY; y < finalY; y++) {
        Vertex left = interpolateVertex(top, botL, y);
        Vertex right = interpolateVertex(top, botR, y);
        scanLine(left, right, y);
    }
}

Vertex Rasterizer::interpolateVertex(Vertex &top, Vertex &bottom, float y) {
    // FIXME: For now use a simple linear interpolation
    assert(top.position.y <= y);
    assert(bottom.position.y >= y);
    assert(top.position.y != bottom.position.y);
    float  t        = (y - top.position.y) / (bottom.position.y - top.position.y);
    Vertex interpolated = Vertex::binomialInterpolation(top, bottom, t);
    assert(glm::epsilonEqual(interpolated.position.y, y, 0.01f));
    interpolated.position.y = y;
    return interpolated;
}

void Rasterizer::scanLine(Vertex &left, Vertex &right, int y) {
    Pixel tempColor (255, 255, 255);
    DepthPixel tempDepth(1);
    int initialX = std::ceil(left.position.x);
    int finalX = std::ceil(right.position.x);
    for (int x = initialX; x < finalX; x++) {
        // TODO: Interpolate the vertex
        renderTarget->checkAndSetPixel(x, y, tempColor, tempDepth);
    }
}

Rasterizer::RasterizerMode Rasterizer::getMode() const {
    return mode;
}

void Rasterizer::setMode(Rasterizer::RasterizerMode mode) {
    Rasterizer::mode = mode;
}

RenderTarget *Rasterizer::getRenderTarget() const {
    return renderTarget;
}

void Rasterizer::setRenderTarget(RenderTarget *renderTarget) {
    Rasterizer::renderTarget = renderTarget;
}

void Rasterizer::calculateWindingOrder(std::vector<Vertex> vertices, std::vector<glm::vec3> &windingOrder) {
    for (int tri = 0; tri < vertices.size() / 3; tri++) {
        Vertex &v1 = vertices[tri * 3];
        Vertex &v2 = vertices[tri * 3 + 1];
        Vertex &v3 = vertices[tri * 3 + 2];

        glm::vec3 v1v2 = v2.position - v1.position;
        glm::vec3 v1v3 = v3.position - v1.position;
        glm::vec3 normal = glm::cross(v1v2, v1v3);
        windingOrder[tri] = normal;
    }
}
