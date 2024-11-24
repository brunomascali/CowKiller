#version 330 core

in vec2 texCoord;
in float outHeight;

out vec4 color;

uniform sampler2D normalMap;

void main()
{
    vec3 ambientLight = vec3(0.1f);

    vec3 normal = texture(normalMap, texCoord).rgb;
    vec3 lightPosition = vec3(0.0f, 10.0f, 0.0f);
    float d = dot(normalize(lightPosition), normalize(normal));

    vec3 terrainColor = vec3(0.45f, 0.7f, 0.22f);

    vec3 finalColor = ambientLight + terrainColor * d;

    color = vec4(finalColor, 1.0f);
} 
