#version 410 core

in vec4 vertexColor;
in vec3 fragPos;
in vec4 normalFrag;
in vec2 textureCoord;
out vec4 color;

struct Material {
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 ambientColor;
    float shine;
    int hasDiffTexture;
    sampler2D diffTexture;
};

uniform Material material;
uniform vec3 viewPos;


subroutine vec3 LightModel(Material material, vec3 normal, vec3 lightDir, vec3 viewDir);

subroutine uniform LightModel LightModelFS;

subroutine (LightModel) vec3 noShade(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    if (material.hasDiffTexture == 1)
        return vec3(texture(material.diffTexture, textureCoord) * vertexColor);
    else
        return vec3(vertexColor);
}

subroutine (LightModel) vec3 gouroudAD(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    return material.hasDiffTexture == 1 ? vec3(texture2D(material.diffTexture, textureCoord)) * vec3(vertexColor) : vec3(vertexColor);
}

subroutine (LightModel) vec3 gouroudADS(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    return material.hasDiffTexture == 1 ? vec3(texture(material.diffTexture, textureCoord) * vertexColor) : vec3(vertexColor);
}

subroutine (LightModel) vec3 phong(Material material, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    vec3 ambient = material.ambientColor;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = material.hasDiffTexture == 1 ? diff * vec3(texture(material.diffTexture, textureCoord)) : diff * material.diffuseColor;
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