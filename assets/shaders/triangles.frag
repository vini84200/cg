#version 330 core

in vec4 vertexColor;
in vec4 normalFrag;
out vec4 color;

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 directionalLight = normalize(vec3(0.5, 0.0, -1.0));

    vec3 ambient = 0.1 * lightColor * vertexColor.xyz;
    vec3 diffuse = lightColor * max(dot(normalize(normalFrag.xyz), directionalLight), 0.0) * vertexColor.xyz;
    color  = vec4(ambient + diffuse, 1.0);
//    color = vertexColor;

//    color = vertexColor;
}