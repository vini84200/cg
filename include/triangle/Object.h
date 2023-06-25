//
// Created by vini84200 on 5/24/23.
//

#ifndef TRIANGLE_OBJECT_H
#define TRIANGLE_OBJECT_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "imgui.h"

enum VAO_IDs {
    ModelSpace, CameraSpace, NumVAOs
};

enum Buffer_IDs {
    PositionBuffer, NormalsBuffer, TextureCoordBuffer, NumBuffers
};

enum Attrib_IDs {
    vPosition = 0, vNormals = 1, vTextureCoord = 2
};

enum IndicesType {
    TriangleType, TriangleStripType, TriangleFanType
};

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shine;

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shine) : ambient(ambient),
                                                                                      diffuse(diffuse),
                                                                                      specular(specular),
                                                                                      shine(shine) {};

    Material() = default;

    void renderImGui() {
        ImGui::ColorEdit3("Ambient", (float *) &ambient);
        ImGui::ColorEdit3("Diffuse", (float *) &diffuse);
        ImGui::ColorEdit3("Specular", (float *) &specular);
        ImGui::DragFloat("Shine", (float *) &shine);
    }
};

#define BUFFER_OFFSET(a) ((void*)(a))

struct CallSpan {
    int start;
    int count;
    int materialIndex;

    CallSpan(int start, int count, int materialIndex) : start(start), count(count), materialIndex(materialIndex) {}
};

struct Vertex {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec2 texture;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture) : position(glm::vec4(position, 1.0)), normal(normal),
                                                                      texture(texture) {};
    Vertex(glm::vec4 position, glm::vec3 normal, glm::vec2 texture) : position(position), normal(normal),
                                                                      texture(texture) {};
    static Vertex binomialInterpolation(Vertex v1, Vertex v2, float t) {
        return Vertex(v1.position * (1 - t) + v2.position * t,
                      v1.normal * (1 - t) + v2.normal * t,
                      v1.texture * (1 - t) + v2.texture * t);
    }
    Vertex() = default;
};

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

    GLuint *getBuffers(int vao);

    GLuint getNumVertices() const;

    void setNumVertices(GLuint numVertices);

    IndicesType getIndecesType() const;

    void setIndecesType(IndicesType indecesType);

    virtual std::string getName() const;

    virtual void renderImGui() = 0;

    virtual void update(float dt) = 0;

    virtual std::vector<CallSpan> getCallSpan() = 0;

    virtual Material *getMaterial(int index) = 0;

    void updateCameraVAO();
    void updateCameraVertices(const glm::mat4 &projMatrix, const glm::mat4 &viewMatrix);

    void addVertex(Vertex vertex);
    void clearVertices();

    std::vector<Vertex> cameraVertices;
protected:
    void initVAO();

private:
    glm::mat4 transformMatrix;
    std::vector<Vertex> originalVertices;
    GLuint VAOs[NumVAOs]{0};
    GLuint buffers[NumVAOs][NumBuffers]{0};
    GLuint numVertices{0};
    GLuint numVisibleVertices{0};
    IndicesType indicesType;
    glm::mat4 lastProjViewMatrix {};

public:
    GLuint getNumVisibleVertices() const;

    void setNumVisibleVertices(GLuint numVisibleVertices);

    bool needsToUpdateCameraVAO;
};

#endif //TRIANGLE_OBJECT_H
