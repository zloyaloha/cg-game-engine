#version 330 core

layout(location = 0) in vec3 aPos;      // Вершины
layout(location = 1) in vec3 aNormal;   // Нормали

out vec3 Normal;   // Нормаль для фрагментного шейдера
out vec3 FragPos;  // Позиция фрагмента для фрагментного шейдера

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));  // Преобразуем позицию вершины в мировой координатный пространство
    Normal = mat3(transpose(inverse(model))) * aNormal;  // Преобразуем нормаль в мировой координатный пространство

    gl_Position = projection * view * vec4(FragPos, 1.0);
}