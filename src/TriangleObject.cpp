//
// Created by vini84200 on 6/1/23.
//

#include "TriangleObject.h"
#include "Window.h"
#include "imgui.h"

TriangleObject::TriangleObject() {
    glGenVertexArrays( NumVAOs, getVaOs() );
    glBindVertexArray( getVaOs()[Triangles] );

    const int numVertices = 3;
    setNumVertices(numVertices);
    setIndecesType(TriangleType);

    GLfloat  vertices[numVertices][3] = {
            { -0.90f, -0.90f, 0.f }, {  0.90, -0.90f, 0.f }, { -0.f,  0.85f, 0.f },  // Triangle 1
    };

    glGenBuffers( NumBuffers, getBuffers() );
    glBindBuffer( GL_ARRAY_BUFFER, getBuffers()[ArrayBuffer] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices),
                  vertices, GL_STATIC_DRAW );

    glVertexAttribPointer( vPosition, 3, GL_FLOAT,
                           GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vPosition );

    GLfloat colors[NumVertices][3] = {
            { 1, 0, 0}, { 0, 1, 0}, { 0, 0, 1},
            { 1, 1, 0}, { 1, 0, 1}, { 0, 1, 1}
    };
    glBindBuffer( GL_ARRAY_BUFFER, getBuffers()[ColorBuffer] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(colors),
                  colors, GL_STATIC_DRAW );

    glVertexAttribPointer( vColor, 3, GL_FLOAT,
                           GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vColor );

    printf("Initialized\n");
}

void TriangleObject::renderImGui() {
    if (ImGui::BeginChild("Triangle")) {
        ImGui::Text("Hello from Triangle");
        ImGui::EndChild();
    }
}

std::string TriangleObject::getName() const {
    return "Triangle";
}

void TriangleObject::update(float dt) {

}
