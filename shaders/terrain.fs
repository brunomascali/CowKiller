#version 330 core

in vec2 texCoord;
in vec3 normal;

uniform sampler2D tex;

out vec4 color;

void main()
{
    vec3 ambientLight = vec3(0.1f);

    vec3 lightPosition = vec3(0.0f, 10.0f, 10.0f);
    float d = dot(normalize(lightPosition), normalize(normal));

    vec4 terrainColor = texture(tex, texCoord);

    vec4 finalColor = terrainColor * d;

    color = vec4(finalColor);
} 
