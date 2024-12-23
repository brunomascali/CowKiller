#version 330 core
layout (location = 0) in vec3 inPosition;

uniform mat4 view;
uniform mat4 projection;

out vec3 texCoord;
out vec3 test;

void main()
{
    texCoord = inPosition;
    vec4 pos = projection * view * vec4(inPosition, 0.0);
    gl_Position = pos.xyww;
    test = inPosition;
}