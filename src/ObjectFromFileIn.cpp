//
// Created by vini84200 on 6/2/23.
//

#include "ObjectFromFileIn.h"
#include "Object.h"
#include "imgui.h"
#include <clocale>
#include <complex>
#include <cstdio>
#include <vector>

ObjectFromFileIn::ObjectFromFileIn(std::string path) {
    printf("ObjectFromFileIn::ObjectFromFileIn\n");
    printf("path: %s\n", path.c_str());
    this->path = path;
    this->name = path.substr(path.find_last_of("/\\") + 1);

    setlocale(LC_ALL, "en_US.UTF-8");

    FILE *fp = fopen(path.c_str(), "r");
    char ch = 'a';
    if (fp == NULL) {
        printf("ERROR: Could not open file %s\n", path.c_str());
    }

    while (ch != '\n') // skip the first line – object’s name
        fscanf(fp, "%c", &ch);
//
    int num_triangles;
    fscanf(fp, "# triangles = %d\n", &num_triangles); // read number of triangles
    fscanf(fp, "Material count = %d\n", &material_count); // read material count
    setNumVertices(num_triangles * 3);
    setIndecesType(TriangleType);
//
    for (int i = 0; i < material_count; i++) {
        Material m;
        printf("Reading material %d\n", i);

        char *first_aux = new char[100];
        char *snd_aux = new char[100];
        char *thrd_aux = new char[100];


        int success_ambient = fscanf(fp, "ambient color %s %s %s \n", first_aux, snd_aux, thrd_aux);
        m.ambient.x = std::stof(first_aux);
        m.ambient.y = std::stof(snd_aux);
        m.ambient.z = std::stof(thrd_aux);
        assert(success_ambient == 3);

        int success_diffuse = fscanf(fp, "diffuse color %s %s %s \n", first_aux, snd_aux, thrd_aux);
        printf("stof: %f %s\n", std::stof(first_aux), first_aux);
        printf("stof: %f\n", std::stof("230.5"));
        m.diffuse.x = std::stof(first_aux);
        m.diffuse.y = std::stof(snd_aux);
        m.diffuse.z = std::stof(thrd_aux);
        assert(success_diffuse == 3);

        int success_specular = fscanf(fp, "specular color %s %s %s \n", first_aux, snd_aux, thrd_aux);
        m.specular.x = std::stof(first_aux);
        m.specular.y = std::stof(snd_aux);
        m.specular.z = std::stof(thrd_aux);
        assert(success_specular == 3);

        int success_shine = fscanf(fp, "material shine %s \n", first_aux);
        m.shine = std::stof(first_aux);
        assert(success_shine == 1);

        materials.push_back(m);
        delete[] first_aux;
        delete[] snd_aux;
        delete[] thrd_aux;

    }
//
    fscanf(fp, "%c", &ch);
    while (ch != '\n') // skip documentation line
        fscanf(fp, "%c", &ch);

    printf("Reading in %s (%d triangles). . .\n", path.c_str(), num_triangles);

    glGenVertexArrays(NumVAOs, getVaOs());
    glBindVertexArray(getVaOs()[Triangles]);

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    GLfloat face_normals[num_triangles][3];

    char *x = new char[100];
    char *y = new char[100];
    char *z = new char[100];
    char *nx = new char[100];
    char *ny = new char[100];
    char *nz = new char[100];
    char *material = new char[100];
    for (int i = 0; i < num_triangles; i++) {
        glm::vec3 vertex, normal, color;
        int material_index = -1;
        int v;
        for (int j = 0; j < 3; j++) {
            int success = fscanf(fp, "v%d %s %s %s %s %s %s %s\n",
                                 &v,
                                 x, y, z,
                                 nx, ny, nz,
                                 material);
            assert(success == 8);
            assert(v == j);
            vertex.x = std::stof(x);
            vertex.y = std::stof(y);
            vertex.z = std::stof(z);
            normal.x = std::stof(nx);
            normal.y = std::stof(ny);
            normal.z = std::stof(nz);

            vertices.push_back(vertex);
            normals.push_back(normal);
            if (material_index == -1)
                material_index = std::stoi(material);
            else
                assert(material_index == std::stoi(material));
        }
        int success = fscanf(fp, "face normal %*s %*s %*s\n");

        if (callSpans.empty() || callSpans.back().materialIndex != material_index) {
            CallSpan span (i*3, 3, material_index);
            callSpans.push_back(span);
        }
        else {
            callSpans.back().count += 3;
        }

    }
    delete[] x;
    delete[] y;
    delete[] z;
    delete[] nx;
    delete[] ny;
    delete[] nz;
    delete[] material;

    printf("Vertices[0]: %f %f %f\n", vertices[0].x, vertices[0].y, vertices[0].z);
    printf("Vertex size: %d\n", vertices.size());
    printf("Difuse of first material: %f %f %f\n", materials[0].diffuse.x, materials[0].diffuse.y,
           materials[0].diffuse.z);


    fclose(fp);

    glGenBuffers( NumBuffers, getBuffers() );
    glBindBuffer( GL_ARRAY_BUFFER, getBuffers()[ArrayBuffer] );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                  vertices.data(), GL_STATIC_DRAW );


    glVertexAttribPointer( vPosition, 3, GL_FLOAT,
                           GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vPosition );

    glBindBuffer( GL_ARRAY_BUFFER, getBuffers()[ColorBuffer] );
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
                 normals.data(), GL_STATIC_DRAW );

    glVertexAttribPointer(vNormals, 3, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray(vNormals );

}

void ObjectFromFileIn::renderImGui() {
    if (ImGui::BeginChild("ObjectFromFileIn")) {
        ImGui::Text("name: %s", name.c_str());
        ImGui::Text("path: %s", path.c_str());
        ImGui::Text("material_count: %d", material_count);
        ImGui::Text("draw_call_count: %zu", callSpans.size());
        ImGui::Text("Triangles: %d", getNumVertices() / 3);
        if (ImGui::BeginChild("Vertices")) {
//        for (int i = 0; i < getNumVertices(); i++) {
//            ImGui::Text("v%d: %f %f %f", i, getVertices()[i].x, getVertices()[i].y, getVertices()[i].z);
//        }
            ImGui::EndChild();
        }
        ImGui::EndChild();
    }
}

std::string ObjectFromFileIn::getName() const {
    return name;
}

void ObjectFromFileIn::update(float dt) {

}

std::vector<CallSpan> ObjectFromFileIn::getCallSpan() {
    return callSpans;
}

Material *ObjectFromFileIn::getMaterial(int index) {
    if (index >= materials.size())
        return nullptr;
    return &materials[index];
}
