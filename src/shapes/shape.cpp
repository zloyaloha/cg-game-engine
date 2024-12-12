#include "shape.h"
#include "iostream"

Shape::Shape(const glm::vec3& shapeColor) : vertexBuffer(QOpenGLBuffer::VertexBuffer), color(shapeColor) {}

void Shape::setViewMatrix(const glm::mat4 &viewMatrix) 
{
    _viewMatrix = viewMatrix;
}

void Shape::setShader(std::shared_ptr<QOpenGLShaderProgram> shader)
{
    shaderProgram = shader;
}

void Shape::setColor(const glm::vec3 &shapeColor)
{
    std::cout << "colores" << std::endl;
    color = shapeColor;
    std::cout << color.r << ' ' << color.g << ' ' << color.b << std::endl;
}

void Shape::setModelMatrix(const glm::mat4 &modelMatrix) 
{
    _modelMatrix = modelMatrix;
}

void Shape::setProjectionMatrix(const glm::mat4 &projectionMatrix) 
{
    _projectionMatrix = projectionMatrix;
}
