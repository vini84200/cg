#ifndef PHONG_PROGRAM_HEADER
#define PHONG_PROGRAM_HEADER

#include "C2GLProgram.h"

class PhongProgram : public C2GLProgram {

  public:
    PhongProgram() = default;

    FragVertex vertexShader(const Vertex &vertex) const override;

    Pixel fragmentShader(const FragVertex &vertex, const FragVertex &dFdx,
                         const FragVertex &dFdy) const override;
};

#endif // DEBUG
