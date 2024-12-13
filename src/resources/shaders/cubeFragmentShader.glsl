#version 420 core

in vec3 Normal;    // Нормаль поверхности
in vec3 FragPos;

const int MAX_LIGHTS = 10;

uniform vec3 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform vec3 lightPointTo[MAX_LIGHTS];
uniform int numLights;

uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 resultColor = vec3(0.0); 

    for (int i = 0; i < numLights; ++i) {
        vec3 lightVec = normalize(lightPos[i]-lightPointTo[i]);

        float diff = max(dot(norm, lightVec), 0.0);
        vec3 diffuse = diff * lightColor[i] * objectColor;

        vec3 ambient = 0.1 * lightColor[i] * objectColor;

        resultColor += diffuse + ambient;
    }

    FragColor = vec4(resultColor, 1.0);
}
