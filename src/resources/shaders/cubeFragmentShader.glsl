#version 420 core

in vec3 Normal;    // Нормаль поверхности
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 lightDir;
uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDirNormalized = normalize(lightDir);
    float diff = max(dot(norm, lightDirNormalized), 0.0);
    
    vec3 diffuse = diff * lightColor * objectColor;
    
    FragColor = vec4(diffuse, 1.0);
}
