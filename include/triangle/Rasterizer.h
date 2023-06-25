//
// Created by vini84200 on 6/23/23.
//

#ifndef TRIANGLE_RASTERIZER_H
#define TRIANGLE_RASTERIZER_H



#include "Object.h"
#include "RenderTarget.h"
#include <vector>
class Rasterizer {
public:
    Rasterizer() = default;

    void renderObject(Object *object);
    void preSortTriangleVertices(std::vector<Vertex> &vertices);
    void drawTriangle(Vertex &top, Vertex &mid, Vertex &bot);

    enum RasterizerMode {
        Wireframe, Solid, Point
    };
private:
    RasterizerMode mode;
    RenderTarget *renderTarget;
public:
    RasterizerMode getMode() const;

    void setMode(RasterizerMode mode);

    RenderTarget *getRenderTarget() const;

    void setRenderTarget(RenderTarget *renderTarget);

private:

    void drawFlatTopTriangle(Vertex &topL, Vertex &vertex1, Vertex &vertex2);
    void drawFlatBottomTriangle(Vertex &top, Vertex &botL, Vertex &botR);
    Vertex interpolateVertex(Vertex &top, Vertex &bottom, float y);
    void scanLine(Vertex &left, Vertex &right, int y);

    void calculateWindingOrder(std::vector<Vertex> vertices, std::vector<glm::vec3> &windingOrder);
};


#endif //TRIANGLE_RASTERIZER_H
