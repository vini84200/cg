#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

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
    gl_Position = position;
    vertexColor = vec4(material.diffuseColor, 1.0f);
    normalFrag  = vec4(normal, 0.0f);
}
