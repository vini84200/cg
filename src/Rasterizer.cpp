//
// Created by vini84200 on 6/23/23.
//

#include "Rasterizer.h"
#include "C2GLProgram.h"
#include "Object.h"
#include "RenderTarget.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/epsilon.hpp"
#include <array>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <tracy/Tracy.hpp>
#include <vector>

void Rasterizer::preSortTriangleVertices(
    std::vector<FragVertex> &vertices) {
    int numTriangles = vertices.size() / 3;
    for (int tri = 0; tri < numTriangles; tri++) {
        FragVertex &v1 = vertices[tri * 3];
        FragVertex &v2 = vertices[tri * 3 + 1];
        FragVertex &v3 = vertices[tri * 3 + 2];
        // Sort the vertices by y. v1 is the top vertex with the
        // lowest y v2 is the middle vertex with the middle y v3 is
        // the bottom vertex with the highest y

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
    ZoneScoped;
    if (this->program == nullptr) {
        throw std::runtime_error("Program is not set");
    }
    C2GLProgram &program = *this->program;
    verticesTempBuffer.clear();
    verticesTempBuffer.reserve(object->getNumVertices());

    const std::vector<Vertex> &vertices
        = object->getOriginalVertices();

    // PHASE 1: Run vertex shader
    for (int i = 0; i < object->getNumVertices(); i += 3) {
        std::array<FragVertex, 3> triangle{};
        // TODO: Use the original vertexes instead of the camera
        // vertexes
        for (int j = 0; j < 3; j++) {
            const Vertex &vertex = vertices[i + j];
            triangle[j]          = program.vertexShader(vertex);
            triangle[j].wInv     = 1;
        }
        bool isTriangleCulled = false;

        // Test culling
        for (int j = 0; j < 3; j++) {
            FragVertex &vertex = triangle[j];
            bool isInFront     = vertex.position.w > 0;
            bool isOutsideInX
                = vertex.position.x < -vertex.position.w
                  || vertex.position.x > vertex.position.w;
            bool isOutsideInY
                = vertex.position.y < -vertex.position.w
                  || vertex.position.y > vertex.position.w;
            bool isOutsideInZ
                = vertex.position.z < -vertex.position.w
                  || vertex.position.z > vertex.position.w;

            isTriangleCulled |= !isInFront || isOutsideInX
                                || isOutsideInY || isOutsideInZ;
            if (isTriangleCulled) {
                break;
            }
        }

        if (isTriangleCulled) {
            continue;
        }

        triangle[0].perpectiveDivide();
        triangle[1].perpectiveDivide();
        triangle[2].perpectiveDivide();

        // Calculate the winding order and cull if necessary
        if (backfaceCulling) {
            FragVertex &v1 = triangle[0];
            FragVertex &v2 = triangle[1];
            FragVertex &v3 = triangle[2];

            glm::vec3 v1v2               = v2.position - v1.position;
            glm::vec3 v1v3               = v3.position - v1.position;
            glm::vec3 windingOrderNormal = glm::cross(v1v2, v1v3);

            if (ccw) {
                if (windingOrderNormal.z < 0) {
                    continue;
                }
            } else {
                if (windingOrderNormal.z > 0) {
                    continue;
                }
            }
        }

        for (int j = 0; j < 3; j++) {
            // Do perspective division
            FragVertex &vertex = triangle[j];

            // Do viewport transform
            vertex.position
                = renderTarget->getViewportMatrix() * vertex.position;

            // Save the vertex to the temp buffer
            verticesTempBuffer.push_back(vertex);
        }
    }

    assert(object->cameraVertices.size() % 3 == 0);

    //    printf("Num triangles: %d\n", object->cameraVertices.size()
    //    / 3);

    preSortTriangleVertices(verticesTempBuffer);

    unsigned long int numTriangles = verticesTempBuffer.size() / 3;

    for (int tri = 0; tri < numTriangles; tri++) {
        FragVertex &v1 = verticesTempBuffer[tri * 3 + 0];
        FragVertex &v2 = verticesTempBuffer[tri * 3 + 1];
        FragVertex &v3 = verticesTempBuffer[tri * 3 + 2];

        switch (mode) {
        case RasterizerMode::Wireframe:
            drawWireframeTriangle(v1, v2, v3);
            break;
        case RasterizerMode::Solid:
            drawTriangle(v1, v2, v3);
            break;
        case RasterizerMode::Point:
            drawPointsTriangle(v1, v2, v3);
            break;
        }
    }
}

void Rasterizer::drawWireframeTriangle(FragVertex &top,
                                       FragVertex &mid,
                                       FragVertex &bot) {
    ZoneScoped;
    // Draw three lines
    drawLine(top, bot);
    drawLine(top, mid);
    drawLine(mid, bot);
}

void Rasterizer::drawLine(FragVertex &start, FragVertex &end) {
    // Use the Bresenham algorithm for line drawing

    int x0 = std::ceil(start.position.x);
    int y0 = std::ceil(start.position.y);
    int x1 = std::ceil(end.position.x) - 1;
    int y1 = std::ceil(end.position.y) - 1;

    int dx  = std::abs(x1 - x0);
    int dy  = std::abs(y1 - y0);
    int sx  = x0 < x1 ? 1 : -1;
    int sy  = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2;
    int e2;
    FragVertex vertex = start;

    while (x0 != x1 || y0 != y1) {
        float t = (((float)x0) - start.position.x)
                  / (end.position.x - start.position.x);
        vertex = FragVertex::interpolate(start, end, t);
        vertex.finish();
        Pixel pixel = program->fragmentShader(vertex);

        renderTarget->checkAndSetPixel(
            x0, y0, ColorPixel(pixel.color), DepthPixel(pixel.depth));

        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y0 += sy;
        }
    }
}

void Rasterizer::drawPointsTriangle(FragVertex &top, FragVertex &mid,
                                    FragVertex &bot) {
    // Set three points
    // Top
    top.finish();
    Pixel pixel = program->fragmentShader(top);
    ColorPixel color(pixel.color);
    DepthPixel depth(pixel.depth);
    renderTarget->checkAndSetPixel(std::ceil(top.position.x),
                                   std::ceil(top.position.y), color,
                                   depth);

    mid.finish();
    pixel = program->fragmentShader(mid);
    color = ColorPixel(pixel.color);
    depth = DepthPixel(pixel.depth);
    renderTarget->checkAndSetPixel(std::ceil(mid.position.x),
                                   std::ceil(mid.position.y), color,
                                   depth);

    bot.finish();
    pixel = program->fragmentShader(bot);
    color = ColorPixel(pixel.color);
    depth = DepthPixel(pixel.depth);
    renderTarget->checkAndSetPixel(std::ceil(bot.position.x),
                                   std::ceil(bot.position.y), color,
                                   depth);
}
void Rasterizer::drawTriangle(FragVertex &top, FragVertex &mid,
                              FragVertex &bot) {
    ZoneScoped;
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
        glm::vec2 a_to_b
            = glm::normalize(top.position - bot.position);
        glm::vec2 w = glm::vec2(a_to_b.y, -a_to_b.x);
        float edge
            = glm::dot(w, glm::vec2(mid.position - bot.position));

        if (edge > 0) {
            // Left triangle
            FragVertex intersection
                = interpolateVertex(top, bot, mid.position.y);
            drawFlatBottomTriangle(top, mid, intersection);
            drawFlatTopTriangle(mid, intersection, bot);
        } else if (edge < 0) {
            // Right triangle
            FragVertex intersection
                = interpolateVertex(top, bot, mid.position.y);
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

void Rasterizer::drawFlatTopTriangle(FragVertex &topL,
                                     FragVertex &topR,
                                     FragVertex &bot) {
    ZoneScoped;
    float heigh     = bot.position.y - topL.position.y;
    int initialY    = std::ceil(topL.position.y);
    int finalY      = std::ceil(bot.position.y);
    glm::vec2 lduv  = (topL.uv - bot.uv) / heigh;
    FragVertex botL = bot;
    botL.duv        = lduv;
    topL.duv        = lduv;
    glm::vec2 rduv  = (topR.uv - bot.uv) / heigh;
    FragVertex botR = bot;
    botR.duv        = rduv;
    topR.duv        = rduv;
    for (int y = initialY; y < finalY; y++) {
        FragVertex left  = interpolateVertex(topL, botL, y);
        FragVertex right = interpolateVertex(topR, botR, y);
        scanLine(left, right, y);
    }
}

void Rasterizer::drawFlatBottomTriangle(FragVertex &top,
                                        FragVertex &botL,
                                        FragVertex &botR) {
    ZoneScoped;
    float heigh     = botL.position.y - top.position.y;
    int initialY    = std::ceil(top.position.y);
    int finalY      = std::ceil(botL.position.y);
    glm::vec2 lduv  = (top.uv - botL.uv) / heigh;
    FragVertex topL = top;
    topL.duv        = lduv;
    botL.duv        = lduv;
    FragVertex topR = top;
    // topL.duv =

    float rduv = (top.uv.s - botR.uv.s) / heigh;
    for (int y = initialY; y < finalY; y++) {
        FragVertex left  = interpolateVertex(top, botL, y);
        FragVertex right = interpolateVertex(top, botR, y);
        scanLine(left, right, y);
    }
}

void Rasterizer::drawFlatTopTriangleWireframe(FragVertex &topL,
                                              FragVertex &topR,
                                              FragVertex &bot) {
    ZoneScoped;
    // TODO: Implement
}

void Rasterizer::drawFlatBottomTriangleWireframe(FragVertex &top,
                                                 FragVertex &botL,
                                                 FragVertex &botR) {
    ZoneScoped;
    // TODO
}

FragVertex Rasterizer::interpolateVertex(FragVertex &top,
                                         FragVertex &bottom,
                                         float y) {
    // FIXME: For now use a simple linear interpolation
    assert(top.position.y <= y);
    assert(bottom.position.y >= y);
    assert(top.position.y != bottom.position.y);
    float t
        = (y - top.position.y) / (bottom.position.y - top.position.y);
    FragVertex interpolated = FragVertex::interpolate(top, bottom, t);
    assert(glm::epsilonEqual(interpolated.position.y, y, 0.01f));
    interpolated.position.y = y;
    return interpolated;
}

void Rasterizer::scanLine(FragVertex &left, FragVertex &right,
                          int y) {
    ZoneScoped;
    int initialX = std::ceil(left.position.x);
    int finalX   = std::ceil(right.position.x);
    for (int x = initialX; x < finalX; x++) {
        float t = (x - left.position.x)
                  / (right.position.x - left.position.x);
        FragVertex interpolated
            = FragVertex::interpolate(left, right, t);
        interpolated.finish();
        Pixel pixel = program->fragmentShader(interpolated);
        ColorPixel color(pixel.color);
        DepthPixel depth(pixel.depth);
        renderTarget->checkAndSetPixel(x, y, color, depth);
    }
}

void Rasterizer::scanLineWireframe(FragVertex &left,
                                   FragVertex &right, int y) {
    ZoneScoped;
    // TODO: Implement
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

C2GLProgram &Rasterizer::getProgram() const {
    if (program == nullptr) {
        throw std::runtime_error("Program not set");
    }
    return *program;
}

void Rasterizer::setProgram(C2GLProgram &program) {
    Rasterizer::program = &program;
}

bool Rasterizer::isBackfaceCulling() const { return backfaceCulling; }

void Rasterizer::setBackfaceCulling(bool backfaceCulling) {
    Rasterizer::backfaceCulling = backfaceCulling;
}

bool Rasterizer::isCcw() const { return ccw; }

void Rasterizer::setCcw(bool ccw) { Rasterizer::ccw = ccw; }
