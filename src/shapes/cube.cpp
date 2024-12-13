#include "cube.h"
#include <QOpenGLFunctions>
#include <glm/gtc/matrix_transform.hpp>
#include <QDebug>
#include <iostream>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

Cube::Cube(float size, const glm::vec3 &position, const glm::vec3& color) : 
    Shape(position, color, "cube"), size(size) {}

void Cube::initialize() {
    generateVertices();
}

void Cube::generateVertices()
{
    // if (vertexBuffer.isCreated()) {
    //     return;  // Если буфер уже создан, не создаем заново
    // }
    float half = size / 2.0f;
    
    glm::vec3 vertices[] = {
        glm::vec3(-half, -half, -half),  // 0
        glm::vec3( half, -half, -half),  // 1
        glm::vec3( half,  half, -half),  // 2
        glm::vec3(-half,  half, -half),  // 3
        glm::vec3(-half, -half,  half),  // 4
        glm::vec3( half, -half,  half),  // 5
        glm::vec3( half,  half,  half),  // 6
        glm::vec3(-half,  half,  half)   // 7
    };

    glm::vec3 vertexData[] = {
        // Задняя грань (нормаль (0.0f, 0.0f, -1.0f))
        vertices[0], glm::vec3(0.0f, 0.0f, -1.0f),
        vertices[1], glm::vec3(0.0f, 0.0f, -1.0f),
        vertices[2], glm::vec3(0.0f, 0.0f, -1.0f),
        vertices[2], glm::vec3(0.0f, 0.0f, -1.0f),
        vertices[3], glm::vec3(0.0f, 0.0f, -1.0f),
        vertices[0], glm::vec3(0.0f, 0.0f, -1.0f),

        // Передняя грань (нормаль (0.0f, 0.0f, 1.0f))
        vertices[4], glm::vec3(0.0f, 0.0f, 1.0f),
        vertices[5], glm::vec3(0.0f, 0.0f, 1.0f),
        vertices[6], glm::vec3(0.0f, 0.0f, 1.0f),
        vertices[6], glm::vec3(0.0f, 0.0f, 1.0f),
        vertices[7], glm::vec3(0.0f, 0.0f, 1.0f),
        vertices[4], glm::vec3(0.0f, 0.0f, 1.0f),

        // Левая грань (нормаль (-1.0f, 0.0f, 0.0f))
        vertices[0], glm::vec3(-1.0f, 0.0f, 0.0f),
        vertices[3], glm::vec3(-1.0f, 0.0f, 0.0f),
        vertices[7], glm::vec3(-1.0f, 0.0f, 0.0f),
        vertices[7], glm::vec3(-1.0f, 0.0f, 0.0f),
        vertices[4], glm::vec3(-1.0f, 0.0f, 0.0f),
        vertices[0], glm::vec3(-1.0f, 0.0f, 0.0f),

        // Правая грань (нормаль (1.0f, 0.0f, 0.0f))
        vertices[1], glm::vec3(1.0f, 0.0f, 0.0f),
        vertices[5], glm::vec3(1.0f, 0.0f, 0.0f),
        vertices[6], glm::vec3(1.0f, 0.0f, 0.0f),
        vertices[6], glm::vec3(1.0f, 0.0f, 0.0f),
        vertices[2], glm::vec3(1.0f, 0.0f, 0.0f),
        vertices[1], glm::vec3(1.0f, 0.0f, 0.0f),

        // Нижняя грань (нормаль (0.0f, -1.0f, 0.0f))
        vertices[0], glm::vec3(0.0f, -1.0f, 0.0f),
        vertices[4], glm::vec3(0.0f, -1.0f, 0.0f),
        vertices[5], glm::vec3(0.0f, -1.0f, 0.0f),
        vertices[5], glm::vec3(0.0f, -1.0f, 0.0f),
        vertices[1], glm::vec3(0.0f, -1.0f, 0.0f),
        vertices[0], glm::vec3(0.0f, -1.0f, 0.0f),

        // Верхняя грань (нормаль (0.0f, 1.0f, 0.0f))
        vertices[3], glm::vec3(0.0f, 1.0f, 0.0f),
        vertices[2], glm::vec3(0.0f, 1.0f, 0.0f),
        vertices[6], glm::vec3(0.0f, 1.0f, 0.0f),
        vertices[6], glm::vec3(0.0f, 1.0f, 0.0f),
        vertices[7], glm::vec3(0.0f, 1.0f, 0.0f),
        vertices[3], glm::vec3(0.0f, 1.0f, 0.0f)
    };

    if (!vao.isCreated()) {
        vao.create();
    }
    vao.bind();

    if (!vertexBuffer.isCreated()) {
        vertexBuffer.create();
    }
    vertexBuffer.bind();
    vertexBuffer.allocate(vertexData, sizeof(vertexData));

    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    if (!f) {
        qDebug() << "Failed to get OpenGL functions.";
        return;
    }

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
    f->glEnableVertexAttribArray(0);

    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    f->glEnableVertexAttribArray(1);

    // vao.release();
    // vertexBuffer.release();
}


void Cube::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void Cube::draw()
{
    if (!shaderProgram->bind()) {
        qDebug() << "Failed to bind shader program.";
        return;
    }
    loadMatriciesToShader();
    loadLightsToShader();
    loadObjectLightToShader();
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    if (!f) {
        qDebug() << "Failed to get OpenGL functions.";
        return;
    }
    vao.bind();
    vertexBuffer.bind();
    if (!QOpenGLContext::currentContext()) {
        qDebug() << "No OpenGL context available.";
    }
    f->glDrawArrays(GL_TRIANGLES, 0, 36);
    // vertexBuffer.release();
    // vao.release();
    shaderProgram->release();
}