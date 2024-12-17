#version 420 core

in vec3 FragPos;  
in vec3 Normal; 
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;  // Позиция камеры
uniform sampler2D texture1;

const int MAX_LIGHTS = 10;

uniform vec3 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform vec3 lightPointTo[MAX_LIGHTS];
uniform int numLights;

uniform vec3 objectColor;

void main() {
     // Нормализация нормали фрагмента
    vec3 norm = normalize(Normal);

    // Получение цвета из текстуры
    vec4 texColor = texture(texture1, TexCoord);

    // Инициализация итогового цвета (изначально амбиентное освещение)
    vec3 resultColor = vec3(0.0);
    
    // Амбиентное освещение: небольшое освещение, чтобы объект не был черным
    for (int i = 0; i < numLights; ++i) {
        // Направление от фрагмента к источнику света
        vec3 lightVec = normalize(lightPos[i] - FragPos);
        
        // Диффузная составляющая (реакция на свет)
        float diff = max(dot(norm, lightVec), 0.0);
        vec3 diffuse = diff * lightColor[i] * objectColor; // Умножаем на цвет объекта (или текстуру)

        // Амбиентное освещение: небольшое освещение от света
        vec3 ambient = 0.1 * lightColor[i] * objectColor;

        // Суммируем результаты для всех источников света
        resultColor += diffuse + ambient;
    }

    // Применяем освещение и текстуру (цвет из текстуры умножается на освещенность)
    FragColor = vec4(resultColor * texColor.rgb, 1); 
    // FragColor = texture(texture1, TexCoord);
}