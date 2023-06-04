//
// Created by vini84200 on 6/2/23.
//

#include "ObjectFromFileIn.h"
#include "Object.h"
#include "imgui.h"
#include <complex>
#include <cstdio>
#include <vector>

ObjectFromFileIn::ObjectFromFileIn(std::string path) {
    printf("ObjectFromFileIn::ObjectFromFileIn\n");
    printf("path: %s\n", path.c_str());
    this->path = path;
    this->name = path.substr(path.find_last_of("/\\") + 1);

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
    int material_index[num_triangles * 3];
    GLfloat face_normals[num_triangles][3];

    for (int i = 0; i < num_triangles; i++) {
        glm::vec3 vertex, normal;
        char *x = new char[100];
        char *y = new char[100];
        char *z = new char[100];
        char *nx = new char[100];
        char *ny = new char[100];
        char *nz = new char[100];
        char *material = new char[100];

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
            // TODO: im overwriting the normals here, but i should be averaging them
            normal = {1.f, 0.f, 0.f};
            material_index[i * 3 + j] = std::stoi(material);
            vertices.push_back(vertex);
            normals.push_back(normal);
        }
        int success = fscanf(fp, "face normal %*s %*s %*s\n");
        delete[] x;
        delete[] y;
        delete[] z;
        delete[] nx;
        delete[] ny;
        delete[] nz;
        delete[] material;
//        assert(success == 3);
    }

    printf("Vertices[0]: %f %f %f\n", vertices[0].x, vertices[0].y, vertices[0].z);
    printf("Vertex size: %d\n", vertices.size());
    printf("normals[0]: %f %f %f\n", normals[0].x, normals[0].y, normals[0].z);
    printf("Normal size: %d\n", normals.size());


    fclose(fp);

    glGenBuffers( NumBuffers, getBuffers() );
    glBindBuffer( GL_ARRAY_BUFFER, getBuffers()[ArrayBuffer] );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                  vertices.data(), GL_STATIC_DRAW );


    glVertexAttribPointer( vPosition, 3, GL_FLOAT,
                           GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vPosition );

    glBindBuffer( GL_ARRAY_BUFFER, getBuffers()[ColorBuffer] );
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                    normals.data(), GL_STATIC_DRAW );

    glVertexAttribPointer( vColor, 3, GL_FLOAT,
                           GL_FALSE, 0, BUFFER_OFFSET(0) );
    glEnableVertexAttribArray( vColor );

}

void ObjectFromFileIn::renderImGui() {
    ImGui::Begin("ObjectFromFileIn");
    ImGui::Text("name: %s", name.c_str());
    ImGui::Text("path: %s", path.c_str());
    ImGui::Text("material_count: %d", material_count);
    ImGui::Text("Triangles: %d", getNumVertices() / 3);
    if (ImGui::BeginChild("Vertices", ImVec2(0, 300), true)) {
//        for (int i = 0; i < getNumVertices(); i++) {
//            ImGui::Text("v%d: %f %f %f", i, getVertices()[i].x, getVertices()[i].y, getVertices()[i].z);
//        }
        ImGui::EndChild();
    }
    ImGui::End();

}

std::string ObjectFromFileIn::getName() const {
    return name;
}

void ObjectFromFileIn::update(float dt) {

}
