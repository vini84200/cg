//
// Created by vini84200 on 6/1/23.
//

#ifndef TRIANGLE_TRIANGLEOBJECT_H
#define TRIANGLE_TRIANGLEOBJECT_H


#include "Object.h"

class TriangleObject : public Object{
public:
    TriangleObject();

    void renderImGui() override;

    std::string getName() const override;
};


#endif //TRIANGLE_TRIANGLEOBJECT_H
