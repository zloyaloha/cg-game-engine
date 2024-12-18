#version 420 core

in vec3 Normal;    // Нормаль поверхности
in vec3 FragPos;

const int MAX_LIGHTS = 10;

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess; 
};

struct Light {
    vec3 lightPos;
    vec3 lightColor;
    vec3 lightPointTo;
};

uniform Material material;
uniform Light lights[MAX_LIGHTS];
uniform int numLights;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 resultColor = vec3(0.0); 

    for (int i = 0; i < numLights; ++i) {
        vec3 lightVec = normalize(lights[i].lightPos - FragPos);

        float diff = max(dot(norm, lightVec), 0.0);
        vec3 diffuse = diff * material.diffuseColor * lights[i].lightColor;

        vec3 ambient = material.ambientColor * lights[i].lightColor;

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(lightVec, norm);
        vec3 halfwayDir = normalize(lightVec + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
        vec3 specular = spec * material.specularColor * lights[i].lightColor;

        resultColor += diffuse + ambient + specular;
    }

    FragColor = vec4(resultColor, 1.0);
}
