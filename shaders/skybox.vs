#version 330 core
  
layout (location = 0) in vec3 inPosition;

uniform mat4 view;
uniform mat4 projection;

out vec3 texCoord;

void main()
{
    texCoord = inPosition;
    vec4 position = projection * view * vec4(inPosition, 1.0f);
    gl_Position = position.xyww;
}