//
// Created by vini84200 on 5/24/23.
//

#include "Object.h"

Object::Object() {

}

void Object::setTransformMatrix(const glm::mat4 &transformMatrix) {
    Object::transformMatrix = transformMatrix;
}

GLuint *Object::getVaOs(){
    return VAOs;
}

GLuint *Object::getBuffers(){
    return Buffers;
}

GLuint Object::getNumVertices() const {
    return numVertices;
}

void Object::setNumVertices(GLuint numVertices) {
    Object::numVertices = numVertices;
}

IndecesType Object::getIndecesType() const {
    return indecesType;
}

void Object::setIndecesType(IndecesType indecesType) {
    Object::indecesType = indecesType;
}

std::string Object::getName() const {

}
