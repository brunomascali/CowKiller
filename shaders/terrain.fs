#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texCoord;

uniform mat4 view;

uniform sampler2D floorTexture;
uniform sampler2D aoTexture;
uniform sampler2D roughnessTexture;

out vec4 color;

void main()
{
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    vec3 textureColor = texture(floorTexture, texCoord).rgb;

    vec3 lightColor = vec3(1.0f);
    vec3 lightPosition = vec3(64.0f, 16.0f, 64.0f);

    vec3 lightDir = normalize(lightPosition - position);
    vec3 cameraDir = normalize(camera_position.xyz - position);
    vec3 halfDir = normalize(lightDir + cameraDir);
    
    float ambientOcclusion = texture(aoTexture, texCoord).r;

    vec3 ambient = textureColor * ambientOcclusion * 4.0;

    float brightness = max(0, dot(normal, lightDir));
    vec3 diffuse = brightness * textureColor * ambientOcclusion;

    float roughness = texture(roughnessTexture, texCoord).r;
    float exponent = mix(1.0, 64.0, 1.0 - roughness);
    float specularTerm = pow(max(dot(normal, halfDir), 0.0), exponent);
    vec3 specular = vec3(0.05) * specularTerm;

    color.rgb = ambient + diffuse + specular;
    color.a = 1.0;
} 
