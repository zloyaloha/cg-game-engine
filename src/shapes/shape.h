#pragma once
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shape {
    public:
        Shape();
        virtual ~Shape() {}

        virtual void initialize() = 0;  // Абстрактный метод для инициализации данных
        virtual void draw() = 0;        // Абстрактный метод для рендеринга

    protected:
        QOpenGLShaderProgram shaderProgram;
        QOpenGLBuffer vertexBuffer;
        QOpenGLVertexArrayObject vao;
        glm::mat4 _modelMatrix;
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;
};
