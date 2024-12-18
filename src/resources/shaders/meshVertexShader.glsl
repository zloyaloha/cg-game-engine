#version 420 core

layout(location = 0) in vec3 aPos;      // Вершины
layout(location = 1) in vec3 aNormal;   // Нормали
layout(location = 2) in vec2 aTexCoord;

out vec3 Normal;   // Нормаль для фрагментного шейдера
out vec3 FragPos;  // Позиция фрагмента для фрагментного шейдера
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalize(vec3(transpose(inverse(model)) * vec4(aNormal, 0.0)));
    TexCoords = aTexCoord;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}