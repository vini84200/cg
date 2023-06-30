//
// Created by vini84200 on 6/23/23.
//

#ifndef TRIANGLE_RASTERIZER_H
#define TRIANGLE_RASTERIZER_H



#include "C2GLProgram.h"
#include "Object.h"
#include "RenderTarget.h"
#include <vector>
class Rasterizer {
public:
    Rasterizer() = default;

    void renderObject(Object *object);
    void preSortTriangleVertices(std::vector<FragVertex> &vertices);
    void drawTriangle(FragVertex &top, FragVertex &mid, FragVertex &bot);

    enum RasterizerMode {
        Wireframe, Solid, Point
    };
private:
    RasterizerMode mode;
    RenderTarget *renderTarget;

    std::vector<FragVertex> verticesTempBuffer;
public:
    RasterizerMode getMode() const;

    void setMode(RasterizerMode mode);

    RenderTarget *getRenderTarget() const;

    void setRenderTarget(RenderTarget *renderTarget);

private:

    void drawFlatTopTriangle(FragVertex &topL, FragVertex &vertex1, FragVertex &vertex2);
    void drawFlatBottomTriangle(FragVertex &top, FragVertex &botL, FragVertex &botR);
    FragVertex interpolateVertex(FragVertex &top, FragVertex &bottom, float y);
    void scanLine(FragVertex &left, FragVertex &right, int y);


    C2GLProgram* program;
    bool ccw;
public:
    bool isCcw() const;

    void setCcw(bool ccw);

public:
    C2GLProgram &getProgram() const;

    void setProgram(C2GLProgram &program);
    bool backfaceCulling = true;
    bool isBackfaceCulling() const;
    void setBackfaceCulling(bool backfaceCulling);
};


#endif //TRIANGLE_RASTERIZER_H
