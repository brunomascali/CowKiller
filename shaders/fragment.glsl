#version 330 core

uniform int textureFlags;
uniform mat4 view;
uniform vec3 camera_position;

uniform sampler2D textureAmbient;
uniform sampler2D textureNormal;
uniform sampler2D textureAO;
uniform sampler2D textureRoughness;
uniform sampler2D textureEmissive;

in vec4 position;
in vec4 normal;
in vec2 texCoord;

out vec4 color;

void main()
{
    bool hasColorMap = (textureFlags & 1) != 0;
    bool hasNormalMap = (textureFlags & 2) != 0;
    bool hasAOMap = (textureFlags & 4) != 0;
    bool hasEmissiveMap = (textureFlags & 8) != 0;

    vec3 normal = normalize(normal.xyz);
    if (hasNormalMap) {
		vec3 normalMap = texture(textureNormal, texCoord).rgb;
		normal = normalize(normalMap * 2.0 - 1.0);
	}

    float ao = 1.0;
    if (hasAOMap) {
		ao = texture(textureAO, texCoord).r;
    }

    float emissionIntensity = 0.7;
    vec3 emission = vec3(0.0f);
    if (hasEmissiveMap) {
        emission = texture(textureEmissive, texCoord).rgb * emissionIntensity;
    }

    vec3 lightColor = vec3(1.0f);
    vec3 lightPosition = vec3(4.0f, 16.0f, 32.0f);

    vec3 lightDir = normalize(lightPosition - position.xyz);
    vec3 cameraDir = normalize(camera_position - position.xyz);
    vec3 halfDir = normalize(lightDir + cameraDir);

    vec3 ambientColor = texture(textureAmbient, texCoord).rgb * ao;

    float brightness = max(0, dot(normal, lightDir));
    vec3 diffuse = brightness * ambientColor;

    float roughness = texture(textureRoughness, texCoord).r;
    float exponent = mix(1.0, 64.0, 1.0 - roughness);
    float specularTerm = pow(max(dot(normal, halfDir), 0.0), exponent);
    vec3 specular = vec3(0.1) * specularTerm;

    color.rgb = (diffuse + specular) * lightColor + emission;
    color.a = 1.0;
}
