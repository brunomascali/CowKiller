#version 330 core

in vec3 normal;

out vec4 color;

void main()
{
    float d = dot(normalize(normal), normalize(vec3(0.0f, 40.0f, -100.0f)));
    vec3 finalColor = vec3(0.9f, 0.5f, 0.3f) * d;
    color = vec4(finalColor, 1.0f);
} 
