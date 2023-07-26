//
// Created by vini84200 on 6/2/23.
//

#ifndef TRIANGLE_OBJECTFROMFILEIN_H
#define TRIANGLE_OBJECTFROMFILEIN_H


#include "Object.h"
#include <vector>


class ObjectFromFileIn : public Object {
  public:
    ObjectFromFileIn(std::string path);
    void renderImGui() override;
    std::string getName() const override;

    std::vector<CallSpan> getCallSpan() override;

    Material *getMaterial(int index) override;

  private:
    std::string path;
    std::string name;

    std::vector<Material> materials;
    std::vector<CallSpan> callSpans;

    int material_count = 0;

  public:
    void update(float dt) override;
};


#endif // TRIANGLE_OBJECTFROMFILEIN_H
