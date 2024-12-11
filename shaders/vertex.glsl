#version 330 core
  
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out vec3 normal;
out vec2 texCoord;

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
    return rotationMatrixX(rotation.x) * rotationMatrixY(rotation.y) * rotationMatrixZ(rotation.z);
}

mat4 translationMatrix(vec3 translation) {
	return mat4(
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            translation.x, translation.y, translation.z, 1.0
        );
}


void main()
{
    normal = inNormal;
    texCoord = inTexCoord;

    gl_Position = projection * view * translationMatrix(translation) * scaleMatrix(scaling) * rotationMatrix(rotation) * vec4(inPosition, 1.0);
}