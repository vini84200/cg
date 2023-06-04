#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 ambientColor;
    float shine;
};

uniform Material material;

out vec4 vertexColor;
out vec4 normalFrag;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vertexColor = vec4(material.diffuseColor, 1.0f);
    normalFrag  = vec4(normal, 0.0f);
}