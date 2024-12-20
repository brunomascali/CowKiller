#version 330 core
  
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec2 texCoord;

void main()
{
    normal = inNormal;
    texCoord = inTexCoord;

    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
}