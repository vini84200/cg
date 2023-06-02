//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_OBJECT_H
#define TRIANGLE_OBJECT_H

#include <glm/glm.hpp>
#include <string>
#include "glad/glad.h"

enum VAO_IDs {
    Triangles, NumVAOs
};
enum Buffer_IDs {
    ArrayBuffer, ColorBuffer, NumBuffers
};
enum Attrib_IDs {
    vPosition = 0, vColor = 1
};

enum IndecesType {
    TriangleType, TriangleStripType, TriangleFanType
};

#define BUFFER_OFFSET(a) ((void*)(a))

/**
 * This class represents an object in the scene.
 * It is responsible for storing the object's data and rendering it.
 */
class Object {
public:
    Object();

    glm::mat4 getTransformMatrix();

    void setTransformMatrix(const glm::mat4 &transformMatrix);

    GLuint *getVaOs();

    GLuint *getBuffers();

    GLuint getNumVertices() const;

    void setNumVertices(GLuint numVertices);

    IndecesType getIndecesType() const;

    void setIndecesType(IndecesType indecesType);
    virtual std::string getName() const;
    virtual void renderImGui() = 0;

private:
    glm::mat4 transformMatrix;
    GLuint VAOs[NumVAOs];
    GLuint Buffers[NumBuffers];
    GLuint numVertices;
    IndecesType indecesType;
};

#endif //TRIANGLE_OBJECT_H
