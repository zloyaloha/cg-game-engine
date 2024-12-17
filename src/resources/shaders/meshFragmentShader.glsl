#version 420 core

in vec3 FragPos;  
in vec3 Normal; 
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;  // Позиция камеры
uniform sampler2D textureSampler;

const int MAX_LIGHTS = 10;

struct Light {
    vec3 lightPos;
    vec3 lightColor;
    vec3 lightPointTo;
};

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

uniform Material material;

void main()
{
    vec3 norm = normalize(Normal);
    
    vec3 texColor = texture(textureSampler, TexCoords).rgb;
    
    vec3 resultColor = vec3(0.0); 

    for (int i = 0; i < numLights; ++i) {
        vec3 lightVec = normalize(lights[i].lightPos - FragPos);

        float diff = max(dot(norm, lightVec), 0.0);
        vec3 diffuse = diff * texColor * material.diffuseColor * lights[i].lightColor;

        vec3 ambient = material.ambientColor * lights[i].lightColor * 0.1;

        vec3 viewDir = normalize(viewPos - FragPos);
        
        vec3 reflectDir = reflect(-lightVec, norm);
        
        float spec = pow(max(dot(norm, viewDir), 0.0), material.shininess);
        vec3 specular = spec * material.specularColor * lights[i].lightColor;

        resultColor += diffuse + ambient + specular;
    }
    resultColor = min(resultColor, vec3(1.0));

    // Выводим итоговый цвет с учетом текстуры и освещения
    FragColor = vec4(resultColor, 1.0);
}