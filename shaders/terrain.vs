#version 330 core
  
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform sampler2D heightmap;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec2 texCoord;
out float outHeight;

void main()
{
    float height = texture(heightmap, inTexCoord).r;
    outHeight = height;

    vec3 pos = vec3(inPosition.x, height * 20.0f, inPosition.z);

    texCoord = inTexCoord;

    gl_Position = projection * view * model * vec4(pos, 1.0f);
}