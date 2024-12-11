#version 330 core

uniform sampler2D tex;

in vec3 normal;
in vec3 ourColor;
in vec2 texCoord;

out vec4 color;

void main()
{
    float d = dot(normalize(normal), normalize(vec3(0.0f, 40.0f, -100.0f)));
    vec3 finalColor = vec3(0.9f, 0.5f, 0.3f) * d;
    // color = vec4(finalColor, 1.0f);

    color = texture(tex, texCoord);
    color.w = 0.5f;
} 
