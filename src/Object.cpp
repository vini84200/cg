//
// Created by vini84200 on 5/24/23.
//

#include "Object.h"
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/vector_relational.hpp"
#include <cmath>


void Object::setTransformMatrix(const glm::mat4 &transformMatrix) {
    Object::transformMatrix = transformMatrix;
}

GLuint *Object::getVaOs() {
    return VAOs;
}

GLuint *Object::getBuffers(int vao) {
    return buffers[vao];
}

GLuint Object::getNumVertices() const {
    return numVertices;
}

void Object::setNumVertices(GLuint numVertices) {
    Object::numVertices = numVertices;
}

IndicesType Object::getIndecesType() const {
    return indicesType;
}

void Object::setIndecesType(IndicesType indecesType) {
    Object::indicesType = indecesType;
}

std::string Object::getName() const {

}

glm::mat4 Object::getTransformMatrix() {
    return glm::mat4();
}

void Object::updateCameraVAO() {
    if (!needsToUpdateCameraVAO)
        return;
    needsToUpdateCameraVAO = false;
    // Bind the VAO
    glBindVertexArray(VAOs[VAO_IDs::CameraSpace]);

    int numVertices = cameraVertices.size();

    glm::vec4 positions[numVertices];
    glm::vec3 normals[numVertices];
    glm::vec2 textures[numVertices];

    for (int i = 0; i < numVertices; i++) {
        positions[i] = cameraVertices[i].position;
        normals[i] = cameraVertices[i].normal;
        textures[i] = cameraVertices[i].texture;
    }

    // Update the buffers
    glBindBuffer(GL_ARRAY_BUFFER, getBuffers(VAO_IDs::CameraSpace)[Buffer_IDs::PositionBuffer]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 4 * sizeof(float), positions);

    glBindBuffer(GL_ARRAY_BUFFER, getBuffers(VAO_IDs::CameraSpace)[Buffer_IDs::NormalsBuffer]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 3 * sizeof(float), normals);

    glBindBuffer(GL_ARRAY_BUFFER, getBuffers(VAO_IDs::CameraSpace)[Buffer_IDs::TextureCoordBuffer]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * 2 * sizeof(float), textures);

    setNumVisibleVertices(numVertices);

    // Unbind the buffer
    glBindVertexArray(0);
}

void Object::initVAO() {
    glGenVertexArrays(NumVAOs, getVaOs());
    glBindVertexArray(getVaOs()[ModelSpace]);

    assert(getNumVertices() == originalVertices.size());
    assert(getNumVertices() > 0);
    float positions[getNumVertices() * 4];
    float normals[getNumVertices() * 3];
    float textures[getNumVertices() * 2];

    for (int i = 0; i < getNumVertices(); i++) {
        positions[i * 4] = originalVertices[i].position.x;
        positions[i * 4 + 1] = originalVertices[i].position.y;
        positions[i * 4 + 2] = originalVertices[i].position.z;
        assert(originalVertices[i].position.w == 1);
        positions[i * 4 + 3] = originalVertices[i].position.w;

        normals[i * 3] = originalVertices[i].normal.x;
        normals[i * 3 + 1] = originalVertices[i].normal.y;
        normals[i * 3 + 2] = originalVertices[i].normal.z;

        textures[i * 2] = originalVertices[i].texture.x;
        textures[i * 2 + 1] = originalVertices[i].texture.y;
    }

    glGenBuffers(NumBuffers, getBuffers(ModelSpace));

    // POSITIONS
    glBindBuffer(GL_ARRAY_BUFFER, getBuffers(0)[PositionBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions),
                 positions, GL_STATIC_DRAW);

    glVertexAttribPointer(vPosition, 4, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);

    // NORMALS
    glBindBuffer(GL_ARRAY_BUFFER, getBuffers(0)[NormalsBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals),
                 normals, GL_STATIC_DRAW);

    glVertexAttribPointer(vNormals, 3, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vNormals);

    // TEXTURES
    glBindBuffer(GL_ARRAY_BUFFER, getBuffers(0)[TextureCoordBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textures),
                 textures, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // CAMERA POSITION BUFFER
    glBindVertexArray(getVaOs()[CameraSpace]);

    glGenBuffers(NumBuffers, getBuffers(CameraSpace));


    // DYNAMIC DRAW FOR POSITION IN CAMERA SPACE
    glBindBuffer(GL_ARRAY_BUFFER, getBuffers(CameraSpace)[PositionBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions),
                 positions, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
    lastProjViewMatrix = glm::mat4();

    // NORMALS
    glBindBuffer(GL_ARRAY_BUFFER, getBuffers(CameraSpace)[NormalsBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals),
                 normals, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vNormals, 3, GL_FLOAT,
                          GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3)));
    glEnableVertexAttribArray(vNormals);

    // TEXTURES
    glBindBuffer(GL_ARRAY_BUFFER, getBuffers(CameraSpace)[TextureCoordBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textures),
                 textures, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(vTextureCoord, 2, GL_FLOAT,
                          GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(sizeof(glm::vec3) * 2));
    glEnableVertexAttribArray(vTextureCoord);

    glBindVertexArray(0);
}

void Object::addVertex(Vertex vertex) {
    originalVertices.push_back(vertex);
    numVertices++;
}

void Object::clearVertices() {
    originalVertices.clear();
    numVertices = 0;
}

GLuint Object::getNumVisibleVertices() const {
    return numVisibleVertices;
}

void Object::setNumVisibleVertices(GLuint numVisibleVertices) {
    Object::numVisibleVertices = numVisibleVertices;
}

Object::Object() {
    transformMatrix = glm::identity<glm::mat4>();

}

void Object::updateCameraVertices(const glm::mat4 &projMatrix, const glm::mat4 &viewMatrix) {
    const glm::mat4 &modelViewMatrix = viewMatrix * transformMatrix;
    const glm::mat4 &modelViewProjMatrix = projMatrix * modelViewMatrix;
    if (lastProjViewMatrix == modelViewProjMatrix && numVisibleVertices > 0) {
        return; // No need to update the VAO
    }
    lastProjViewMatrix = projMatrix;
    needsToUpdateCameraVAO = true;

    assert(numVertices == originalVertices.size());
    assert(numVertices % 3 == 0); // We only support triangles for now


    const glm::mat4 &normalMatrixIn4 = glm::transpose(glm::inverse(modelViewMatrix));
    const glm::mat3 &normalMatrix = glm::mat3(normalMatrixIn4);

    int numTriangles = numVertices / 3;
    bool keep = true;
    int numVerticesToKeep = -3;

    cameraVertices.clear();

    // Calculate new vertices
    for (int tri = 0; tri < numTriangles; tri++) {
        // If we are keeping the triangle then we increase the number of vertices to keep
        // Otherwise the triangle will be overwritten by the next one
        if (keep) {
            numVerticesToKeep += 3;
            cameraVertices.emplace_back();
            cameraVertices.emplace_back();
            cameraVertices.emplace_back();
        }

        keep = true;

        for (int i = 0; i < 3; i++) {
            int index = tri * 3 + i;
            int newIndex = numVerticesToKeep + i;

            assert(index < numVertices);
            assert(originalVertices[index].position.w == 1);
            cameraVertices[newIndex].position = modelViewProjMatrix * originalVertices[index].position;

            if (cameraVertices[newIndex].position.w <= 0 || std::isnan(cameraVertices[newIndex].position.w)) {
                // If the vertex is behind the camera or is invalid we don't keep the triangle
                keep = false;
                break;
            }

            cameraVertices[newIndex].position /= cameraVertices[newIndex].position.w;

            if (std::abs(cameraVertices[newIndex].position.x) > 1 ||
                std::abs(cameraVertices[newIndex].position.y) > 1 ||
                std::abs(cameraVertices[newIndex].position.z) > 1) {
                // If the vertex is outside the screen we don't keep the triangle
                keep = false;
                break;
            }

            cameraVertices[newIndex].normal = normalMatrix * originalVertices[index].normal;
            cameraVertices[newIndex].texture = originalVertices[index].texture;
        }
    }

    if (!keep) {
        // If the last triangle is not valid we don't keep it
        numVerticesToKeep -= 3;
        cameraVertices.pop_back();
        cameraVertices.pop_back();
        cameraVertices.pop_back();
    }

}
