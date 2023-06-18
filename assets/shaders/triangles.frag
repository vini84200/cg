#version 410 core

in vec4 vertexColor;
in vec3 fragPos;
in vec4 normalFrag;
out vec4 color;

struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 ambientColor;
    float shine;
};

uniform Material material;
uniform vec3 viewPos;


subroutine vec3 LightModel(Material material, vec3 normal, vec3 lightDir, vec3 viewDir);

subroutine uniform LightModel LightModelFS;

subroutine (LightModel) vec3 noShade(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    return vec3(vertexColor);
}

subroutine (LightModel) vec3 gouroudAD(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    return vec3(vertexColor);
}

subroutine (LightModel) vec3 gouroudADS(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    return vec3(vertexColor);
}

subroutine (LightModel) vec3 phong(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    vec3 ambient = material.ambientColor;
    vec3 diffuse = material.diffuseColor * max(dot(normal, lightDir), 0.0);
    float energyConservation = (material.shine + 8.0f) / (8.0f * 3.14159f);
    vec3 halfDir = normalize(lightDir + viewDir);
    vec3 specular = material.specularColor * pow(max(dot(normal, halfDir), 0.0), material.shine) * energyConservation;
    return ambient + diffuse + specular;
}

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 directionalLight = normalize(vec3(0.0, 0.0, 1.0));

    color  = vec4(LightModelFS(material, normalize(normalFrag.xyz), directionalLight, normalize(viewPos - fragPos)), 1.0);
}