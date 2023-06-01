//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_MESHBUNDLE_H
#define TRIANGLE_MESHBUNDLE_H


#include "glad/glad.h"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, ColorBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0, vColor = 1 };

class MeshBundle {
public:
    MeshBundle();

    MeshBundle fromFile(const char* path);


private:
    GLuint NumVertices = 6;

};


#endif //TRIANGLE_MESHBUNDLE_H
