#version 330 core

#define GRIMM_ID 1
#define DRAGON_ID 2
#define PLANET_ID 3
#define SUN_ID 4

uniform int currentModel;
uniform int textureFlags;
uniform mat4 view;
uniform vec3 camera_position;

uniform sampler2D textureAmbient;
uniform sampler2D textureNormal;
uniform sampler2D textureAO;
uniform sampler2D textureRoughness;
uniform sampler2D textureEmissive;

in vec4 position_world;
in vec4 position_model;
in vec4 normal;
in vec3 center;
in vec2 texCoord;

out vec4 color;

#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{
	bool hasColorMap = (textureFlags & 1) != 0;
	bool hasNormalMap = (textureFlags & 2) != 0;
	bool hasAOMap = (textureFlags & 4) != 0;
	bool hasEmissiveMap = (textureFlags & 8) != 0;

	vec2 texCoord = texCoord;

	if (currentModel == PLANET_ID || currentModel == SUN_ID) {
		vec4 bbox_center = vec4(center, 0.0f);
		vec4 p_prime = bbox_center + (position_world - bbox_center) / length(position_world - bbox_center);
		vec4 vec_p = normalize(position_world - bbox_center);

		float theta = atan(vec_p.x, vec_p.z);
		float phi = asin(vec_p.y);

		texCoord.x = (theta + M_PI) / (2 * M_PI);
		texCoord.y = (phi + M_PI_2) / M_PI;
	}

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

	if (currentModel != SUN_ID) {
		vec3 lightColor = vec3(1.0f);
		vec3 lightPosition = vec3(64.0f, 16.0f, 64.0f);

		vec3 lightDir = normalize(lightPosition - position_world.xyz);
		vec3 cameraDir = normalize(camera_position - position_world.xyz);
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
	else {
		color.rgb = texture(textureAmbient, texCoord).rgb;
		color.a = 1.0;
	}
}
