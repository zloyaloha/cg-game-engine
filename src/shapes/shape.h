#pragma once
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shape {
    public:
        Shape(const glm::vec3& color);
        virtual ~Shape() {}

        virtual void initialize() = 0;  // Абстрактный метод для инициализации данных
        virtual void draw() = 0;        // Абстрактный метод для рендеринга
        void setModelMatrix(const glm::mat4& modelMatrix);
        void setProjectionMatrix(const glm::mat4& projectionMatrix);
        void setViewMatrix(const glm::mat4& viewMatrix);
        void setShader(std::shared_ptr<QOpenGLShaderProgram> shader);
        void setColor(const glm::vec3& color);
    protected:
        std::shared_ptr<QOpenGLShaderProgram> shaderProgram;
        QOpenGLBuffer vertexBuffer;
        QOpenGLVertexArrayObject vao;
        glm::vec3 color;
        glm::mat4 _modelMatrix;
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;
};
