#version 330 core
  
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 view;
uniform mat4 projection;

uniform sampler2D displacement;

out vec3 position;
out vec3 normal;
out vec2 texCoord;

void main()
{
    position = inPosition;
    normal = inNormal;
    texCoord = inTexCoord;

    vec3 finalPosition = inPosition;
    finalPosition = finalPosition + inNormal * texture(displacement, inTexCoord).r * 0.25;
    gl_Position = projection * view * vec4(finalPosition, 1.0f);
}