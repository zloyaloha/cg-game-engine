#include "cube.h"
#include <QOpenGLFunctions>
#include <glm/gtc/matrix_transform.hpp>
#include <QDebug>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

Cube::Cube(float size, const glm::vec3 &position) : 
    size(size), position(position) {}

void Cube::initialize() {
    generateVertices();

    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/cubeVertexShader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/cubeFragmentShader.glsl");
    if (!shaderProgram.link()) {
        qDebug() << "Shader linking failed: " << shaderProgram.log();
    }

}

void Cube::generateVertices()
{
    GLfloat halfSize = size / 2.0f;

    vertices = {
        glm::vec3(-halfSize, -halfSize, -halfSize),  // 0
        glm::vec3( halfSize, -halfSize, -halfSize),  // 1
        glm::vec3( halfSize,  halfSize, -halfSize),  // 2
        glm::vec3(-halfSize,  halfSize, -halfSize),  // 3
        glm::vec3(-halfSize, -halfSize,  halfSize),  // 4
        glm::vec3( halfSize, -halfSize,  halfSize),  // 5
        glm::vec3( halfSize,  halfSize,  halfSize),  // 6
        glm::vec3(-halfSize,  halfSize,  halfSize)   // 7
    };

    glm::vec3 vertexData[] = {
        // Задняя грань (нормаль по оси Z)
        // Первый треугольник
        vertices[0], glm::vec3(0.0f, 0.0f, -1.0f),
        vertices[1], glm::vec3(0.0f, 0.0f, -1.0f),
        vertices[2], glm::vec3(0.0f, 0.0f, -1.0f),
        // Второй треугольник
        vertices[2], glm::vec3(0.0f, 0.0f, -1.0f),
        vertices[3], glm::vec3(0.0f, 0.0f, -1.0f),
        vertices[0], glm::vec3(0.0f, 0.0f, -1.0f),

        // Левая грань (нормаль по оси X)
        // Первый треугольник
        vertices[0], glm::vec3(-1.0f, 0.0f, 0.0f),
        vertices[3], glm::vec3(-1.0f, 0.0f, 0.0f),
        vertices[7], glm::vec3(-1.0f, 0.0f, 0.0f),
        // Второй треугольник
        vertices[7], glm::vec3(-1.0f, 0.0f, 0.0f),
        vertices[4], glm::vec3(-1.0f, 0.0f, 0.0f),
        vertices[0], glm::vec3(-1.0f, 0.0f, 0.0f),

        // Правая грань (нормаль по оси X)
        // Первый треугольник
        vertices[1], glm::vec3(1.0f, 0.0f, 0.0f),
        vertices[2], glm::vec3(1.0f, 0.0f, 0.0f),
        vertices[6], glm::vec3(1.0f, 0.0f, 0.0f),
        // Второй треугольник
        vertices[6], glm::vec3(1.0f, 0.0f, 0.0f),
        vertices[5], glm::vec3(1.0f, 0.0f, 0.0f),
        vertices[1], glm::vec3(1.0f, 0.0f, 0.0f),

        // Нижняя грань (нормаль по оси Y)
        // Первый треугольник
        vertices[0], glm::vec3(0.0f, -1.0f, 0.0f),
        vertices[1], glm::vec3(0.0f, -1.0f, 0.0f),
        vertices[5], glm::vec3(0.0f, -1.0f, 0.0f),
        // Второй треугольник
        vertices[5], glm::vec3(0.0f, -1.0f, 0.0f),
        vertices[4], glm::vec3(0.0f, -1.0f, 0.0f),
        vertices[0], glm::vec3(0.0f, -1.0f, 0.0f),

        // Верхняя грань (нормаль по оси Y)
        // Первый треугольник
        vertices[3], glm::vec3(0.0f, 1.0f, 0.0f),
        vertices[2], glm::vec3(0.0f, 1.0f, 0.0f),
        vertices[6], glm::vec3(0.0f, 1.0f, 0.0f),
        // Второй треугольник
        vertices[6], glm::vec3(0.0f, 1.0f, 0.0f),
        vertices[7], glm::vec3(0.0f, 1.0f, 0.0f),
        vertices[3], glm::vec3(0.0f, 1.0f, 0.0f),

        // Передняя грань (нормаль по оси Z)
        // Первый треугольник
        vertices[4], glm::vec3(0.0f, 0.0f, 1.0f),
        vertices[5], glm::vec3(0.0f, 0.0f, 1.0f),
        vertices[6], glm::vec3(0.0f, 0.0f, 1.0f),
        // Второй треугольник
        vertices[6], glm::vec3(0.0f, 0.0f, 1.0f),
        vertices[7], glm::vec3(0.0f, 0.0f, 1.0f),
        vertices[4], glm::vec3(0.0f, 0.0f, 1.0f)
    };
    vao.create();
    vao.bind();

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertexData, sizeof(vertexData));

    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    if (!f) {
        qDebug() << "Failed to get OpenGL functions.";
        return;
    }

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    f->glEnableVertexAttribArray(0);

    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    f->glEnableVertexAttribArray(1);

    vao.release();
    vertexBuffer.release();
}


void Cube::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

void Cube::draw()
{
    if (!shaderProgram.bind()) {
        qDebug() << "Failed to bind shader program.";
        return;
    }
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);

    shaderProgram.setUniformValue("view", QMatrix4x4(glm::value_ptr(_viewMatrix)).transposed());
    shaderProgram.setUniformValue("model", QMatrix4x4(glm::value_ptr(modelMatrix)).transposed());
    shaderProgram.setUniformValue("projection", QMatrix4x4(glm::value_ptr(_projectionMatrix)).transposed());
    vao.bind();
    if (!QOpenGLContext::currentContext()) {
        qDebug() << "No OpenGL context available.";
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);
    vao.release();
    shaderProgram.release();
    
}

void Cube::setViewMatrix(const glm::mat4 &viewMatrix) 
{
    _viewMatrix = viewMatrix;
}

void Cube::setModelMatrix(const glm::mat4 &modelMatrix) 
{
    _modelMatrix = modelMatrix;
}

void Cube::setProjectionMatrix(const glm::mat4 &projectionMatrix) 
{
    _projectionMatrix = projectionMatrix;
}
