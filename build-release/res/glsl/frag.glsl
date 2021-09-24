#version 420

out vec4 color;

in vec3 fragPos;
in vec3 _normal;

in vec2 pass_texCoord;
in vec4 _fragPosLightSpace;
layout(binding = 0) uniform sampler2D u_texture;
layout(binding = 2) uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform int isPicked;

const float kPi = 3.14159265;
const float kShininess = 16.0;

float ShadowCalculation()
{
    vec3 projCoords = _fragPosLightSpace.xyz / _fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    vec3 normal = normalize(_normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    bias = 0.0005;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
    shadow = 0.0;

    return 1 - shadow;
}

float when_gt(float x, float y) {
    return max(sign(x - y), 0.0);
}

void main()
{
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    float ambientStrength = 0.45f;
    vec3 ambientLighting = ambientStrength * lightColor;

    vec3 norm = normalize(_normal);
    vec3 lightDirection = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDirection), 0.0f);
    vec3 diffuse = diff * lightColor * 0.6f;

    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - fragPos);

    /*const float kEnergyConservation = ( 2.0 + kShininess ) / ( 2.0 * kPi );
    vec3 reflectDir = reflect(-lightDirection, _normal);
    float spec = kEnergyConservation * pow(max(dot(viewDir, reflectDir), 0.0), kShininess) * when_gt(diff, 0.0f);*/

    const float kEnergyConservation = ( 8.0 + kShininess ) / ( 8.0 * kPi );
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    float spec = kEnergyConservation * pow(max(dot(_normal, halfwayDir), 0.0), kShininess) * when_gt(diff, 0.0f);
    /*vec3 reflectDir = reflect(-lightDirection, _normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);*/
    vec3 specular = spec * lightColor * specularStrength;
    //specular = vec3(0.0);
    float shadow = ShadowCalculation();


    //float constant = 1.0f;
    //float linear = 0.0014;
    //float quadratic = 0.000007;
    //float distance = length(lightPos - fragPos);
    //float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    //ambientLighting *= attenuation;
    //diffuse *= attenuation;
    //specular *= attenuation;
    //shadow = 1;
    color = (vec4(ambientLighting, 1) + shadow * (vec4(diffuse, 1) + vec4(specular, 1))) * texture(u_texture, pass_texCoord);
    if (isPicked == 1) color = vec4(1.0, 0.0, 0.0, 1.0);
    //color = vec4(dot(_normal, halfwayDir));
    //color = vec4(_normal, 1.0);
}
