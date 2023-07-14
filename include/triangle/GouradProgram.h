#ifndef GOURAD_PROGRAM_HEADER
#define GOURAD_PROGRAM_HEADER

#include "C2GLProgram.h"

class GouradProgram : public C2GLProgram {

public:
    GouradProgram() = default;

    FragVertex vertexShader(const Vertex &vertex) const override;

    Pixel fragmentShader(FragVertex &vertex) override;

    void updateNormalMatrix() override;
};

#endif // DEBUG
