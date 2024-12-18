#version 420 core

in vec3 Normal;
in vec3 FragPos;

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess; 
};

const int MAX_POINT_LIGHTS = 10;
const int MAX_SPOT_LIGHTS = 10;
const int MAX_DIRECTIONAL_LIGHTS = 10;

struct PointLight {
    vec3 lightPos;
    vec3 lightColor;
    float intensity;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 lightPos;
    vec3 lightColor;
    vec3 lightDir;
    float intensity;
    float cutOff;
    float outerCutOff;
};

struct DirectionalLight {
    vec3 lightDir;
    vec3 lightColor;
    float intensity;
};

uniform Material material;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform DirectionalLight dirLights[MAX_DIRECTIONAL_LIGHTS];

uniform int numPointLights;
uniform int numSpotLights;
uniform int numDirLights;

uniform vec3 viewPos;

out vec4 FragColor;

vec3 pointLightLighting(PointLight light, vec3 fragPos, vec3 norm, Material material, vec3 viewPos) {
    vec3 lightVec = fragPos - light.lightPos;
    float distance = length(lightVec);
    lightVec = normalize(lightVec);

    float diff = max(dot(norm, lightVec), 0.0);
    vec3 diffuse = diff * material.diffuseColor * light.lightColor * light.intensity;

    vec3 ambient = material.ambientColor * light.lightColor * 0.1;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightVec, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * material.specularColor * light.lightColor * light.intensity;

    float attenuation = 1.0 / (light.constant * distance * (distance * distance));

    return (ambient + diffuse + specular) * attenuation;
}

vec3 spotLightLighting(SpotLight light, vec3 fragPos, vec3 norm, Material material, vec3 viewPos) {
    vec3 lightVec = fragPos - light.lightPos;
    float distance = length(lightVec);
    lightVec = normalize(lightVec);

    float diff = max(dot(norm, lightVec), 0.0);
    vec3 diffuse = diff * material.diffuseColor * light.lightColor * light.intensity;

    vec3 ambient = material.ambientColor * light.lightColor * 0.1;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightVec, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * material.specularColor * light.lightColor * light.intensity;

    float theta = dot(lightVec, normalize(-light.lightDir));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensityFactor = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 result = (ambient + diffuse + specular) * intensityFactor;
    return result;
}

vec3 directionalLightLighting(DirectionalLight light, vec3 fragPos, vec3 norm, Material material, vec3 viewPos) {
    vec3 lightVec = normalize(-light.lightDir);

    float diff = max(dot(norm, lightVec), 0.0);
    vec3 diffuse = diff * material.diffuseColor * light.lightColor * light.intensity;

    vec3 ambient = material.ambientColor * light.lightColor * 0.1;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightVec, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * material.specularColor * light.lightColor * light.intensity;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = normalize(Normal);
    
    vec3 resultColor = vec3(0.0); 

    for (int i = 0; i < numPointLights; ++i) {
        resultColor += pointLightLighting(pointLights[i], FragPos, norm, material, viewPos);
    }

    for (int i = 0; i < numSpotLights; ++i) {
        resultColor += spotLightLighting(spotLights[i], FragPos, norm, material, viewPos);
    }

    for (int i = 0; i < numDirLights; ++i) {
        resultColor += directionalLightLighting(dirLights[i], FragPos, norm, material, viewPos);
    }

    FragColor = vec4(resultColor, 1.0);
}
