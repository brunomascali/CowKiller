#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D normalMap;

void main()
{
    vec3 normal = texture(normalMap, texCoord).rgb;

    vec3 lightPosition = vec3(0.0f, 10.0f, 0.0f);

    float d = dot(normalize(lightPosition), normalize(normal));

    color = vec4(0.5f * d);
} 
