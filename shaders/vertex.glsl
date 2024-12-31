#version 330 core
  
// Location precisam ser os mesmo dos especificados no enum Location em mesh.hpp
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out vec4 position_world;
out vec4 position_model;
out vec4 normal;
out vec2 texCoord;
out vec3 center;

uniform vec3 translation;
uniform vec3 rotation;
uniform float scaling;

uniform mat4 view;
uniform mat4 projection;

mat4 scaleMatrix(float s) {
    return mat4(
		s, 0.0f, 0.0f, 0.0f,
		0.0f, s, 0.0f, 0.0f,
		0.0f, 0.0f, s, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
    );
}

mat4 rotationMatrixX(float angle) {
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, cosTheta, -sinTheta, 0.0,
        0.0, sinTheta, cosTheta, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotationMatrixY(float angle) {
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    return mat4(
        cosTheta, 0.0, sinTheta, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sinTheta, 0.0, cosTheta, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotationMatrixZ(float angle) {
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    return mat4(
        cosTheta, -sinTheta, 0.0, 0.0,
        sinTheta, cosTheta, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

mat4 rotationMatrix(vec3 rotation) {
    return rotationMatrixY(rotation.y) * rotationMatrixZ(rotation.z) * rotationMatrixX(rotation.x);
}

mat4 translateMatrix(vec3 translation) {
return mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		translation.x, translation.y, translation.z, 1.0
	);
}

void main()
{
    texCoord = inTexCoord;

    mat4 model = translateMatrix(translation) * scaleMatrix(scaling) * rotationMatrix(rotation);

    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
    position_world = model * vec4(inPosition, 1.0f);
    position_model = vec4(inPosition, 1.0f);
    normal = model * vec4(inNormal, 0.0);
    center = translation;
}