//
// Created by vini84200 on 6/1/23.
//

#ifndef TRIANGLE_TRIANGLEOBJECT_H
#define TRIANGLE_TRIANGLEOBJECT_H


#include "Object.h"
#include "ObjectFromFileIn.h"

class TriangleObject : public Object {
  public:
    TriangleObject();

    void renderImGui() override;

    std::string getName() const override;

    void update(float dt) override;

    Material *getMaterial(int index) override;

    std::vector<CallSpan> getCallSpan() override;

    Material material_;
};


#endif // TRIANGLE_TRIANGLEOBJECT_H
