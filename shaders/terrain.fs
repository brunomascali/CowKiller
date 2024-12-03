#version 330 core

in vec2 texCoord;
in vec3 normal;

out vec4 color;

void main()
{
    vec3 ambientLight = vec3(0.1f);

    vec3 lightPosition = vec3(0.0f, 10.0f, 10.0f);
    float d = dot(normalize(lightPosition), normalize(normal));

    vec3 terrainColor = vec3(0.8f, 0.8f, 0.8f);

    vec3 finalColor = terrainColor * d;

    color = vec4(finalColor, 1.0f);
} 
