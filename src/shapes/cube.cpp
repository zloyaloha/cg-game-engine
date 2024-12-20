#include "cube.h"
#include <QOpenGLFunctions>
#include <glm/gtc/matrix_transform.hpp>
#include <QDebug>
#include <iostream>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

Cube::Cube(float size, const glm::vec3 &position) :
    Shape(position, "cube"), size(size) {}


void Cube::generateVertices()
{
    float half = size / 2.0f;
    vertices = {
        glm::vec3(-half, half, half),
        glm::vec3(-half, half, -half),
        glm::vec3(-half, -half, -half),
        glm::vec3(-half, -half, half),
        glm::vec3(-half, half, -half),
        glm::vec3(half, half, -half),
        glm::vec3(half, -half, -half),
        glm::vec3(-half, -half, -half),
        glm::vec3(half, half, -half),
        glm::vec3(half, half, half),
        glm::vec3(half, -half, half),
        glm::vec3(half, -half, -half),
        glm::vec3(half, half, half),
        glm::vec3(-half, half, half),
        glm::vec3(-half, -half, half),
        glm::vec3(half, -half, half),
        glm::vec3(-half, -half, half),
        glm::vec3(-half, -half, -half),
        glm::vec3(half, -half, -half),
        glm::vec3(half, -half, half),
        glm::vec3(half, half, half),
        glm::vec3(half, half, -half),
        glm::vec3(-half, half, -half),
        glm::vec3(-half, half, half)
    };

    // Для каждой вершины должны быть нормали
    normals = {
        glm::vec3(-1, -0, 0),
        glm::vec3(-1, 0, -0),
        glm::vec3(-1, -0, 0),
        glm::vec3(-1, -0, 0),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, -0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0)
    };

    indices = {
        0, 1, 2,  0, 2, 3, // Задняя грань
        4, 5, 6,  4, 6, 7, // Передняя грань
        8, 9, 10,  8, 10, 11, // Левая грань
        12, 13, 14,  12, 14, 15, // Правая грань
        16, 17, 18,  16, 18, 19, // Нижняя грань
        20, 21, 22,  20, 22, 23  // Верхняя грань
    };
    initAABB();
}

void Cube::initialize() {
    generateVertices();

    shaderProgram->bind();
    _vao->bind();
    pos = shaderProgram->attributeLocation("aPos");
    _normal = shaderProgram->attributeLocation("aNormal");

    normalBuffer.create();
    normalBuffer.bind();
    normalBuffer.allocate(&normals[0], normals.size() * sizeof(glm::vec3));
    normalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    shaderProgram->setAttributeBuffer(_normal, GL_FLOAT, 0, 3);
    shaderProgram->enableAttributeArray(_normal);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(&vertices[0], vertices.size() * sizeof(glm::vec3));
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    shaderProgram->setAttributeBuffer(pos, GL_FLOAT, 0, 3);
    shaderProgram->enableAttributeArray(pos);

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(&indices[0], indices.size() * sizeof(unsigned int));
    indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vao->release();
}

void Cube::draw()
{
    this->setMaterial(material);
    shaderProgram->bind();
    _vao->bind();

    vertexBuffer.bind();
    shaderProgram->setAttributeBuffer(pos, GL_FLOAT, 0, 3);
    shaderProgram->enableAttributeArray(pos);

    normalBuffer.bind();
    shaderProgram->setAttributeBuffer(_normal, GL_FLOAT, 0, 3);
    shaderProgram->enableAttributeArray(_normal);

    indexBuffer.bind();

    loadMatriciesToShader();
    loadLightsToShader();
    loadMaterialToShader();

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    _vao->release();
    shaderProgram->release();
}