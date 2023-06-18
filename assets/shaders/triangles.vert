#version 410 core

layout (location = 0) in vec4 position;
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
out vec3 fragPos;
out vec4 normalFrag;

subroutine vec3 LightModel(Material material, vec3 normal, vec3 lightDir, vec3 viewDir);

subroutine uniform LightModel lightModelVS;

subroutine (LightModel) vec3 noShade(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    vec3 diffuse = material.diffuseColor;
//    return vec3(1.0f, 0.0f, 0.0f);
    return diffuse;
}

subroutine (LightModel) vec3 gouroudAD(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    vec3 ambient = material.ambientColor;
    vec3 diffuse = material.diffuseColor * max(dot(normal, lightDir), 0.0f);
//    return vec3(normal);
    return ambient + diffuse;
}

subroutine (LightModel) vec3 gouroudADS(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    vec3 ambient = material.ambientColor;
    vec3 diffuse = material.diffuseColor * max(dot(normal, lightDir), 0.0f);
    float energyConservation = (material.shine + 8.0f) / (8.0f * 3.14159f);
    vec3 halfDir = normalize(lightDir + viewDir);
    vec3 specular = max(vec3(0,0,0), material.specularColor * pow(max(dot(normal, halfDir), 0.0f), material.shine) * energyConservation);
    return ambient + diffuse + specular;
}

subroutine (LightModel) vec3 phong(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    return vec3(0.0f);
}
uniform vec3 viewPos;

void main()
{
    gl_Position = projection * view * model * position;
    fragPos = vec3(model * position);
    vertexColor = vec4(lightModelVS(material, normalize(normal), vec3(0.0f, 0.8f, 0.2f), normalize(viewPos - fragPos)), 1.0f);
    normalFrag  = vec4(normal, 0.0f);
}
