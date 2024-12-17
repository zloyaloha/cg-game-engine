#pragma once
#include <vector>
#include <string>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Light {
    glm::vec3 position;
    glm::vec3 pointTo;
    glm::vec3 color;
};

struct Texture {
    QOpenGLTexture* texture;
    aiTextureType type;
};

class Shape {
    public:
        Shape(const glm::vec3& position, const glm::vec3& color, const std::string& type);
        virtual ~Shape() {}

        virtual void initialize() = 0;  // Абстрактный метод для инициализации данных
        virtual void draw() = 0;        // Абстрактный метод для рендеринга
        void setModelMatrix(const glm::mat4& modelMatrix);
        void setProjectionMatrix(const glm::mat4& projectionMatrix);
        void setViewMatrix(const glm::mat4& viewMatrix);
        void setShader(std::shared_ptr<QOpenGLShaderProgram> shader);
        void setColor(const glm::vec3& color);
        void loadMatriciesToShader();
        void loadLightsToShader();
        void loadObjectLightToShader();
        void setLights(const std::vector<std::shared_ptr<Light>>& light);
        std::string getType() const;
    protected:
        std::vector<std::shared_ptr<Light>> lights;
        glm::vec3 position;
        std::shared_ptr<QOpenGLShaderProgram> shaderProgram;
        QOpenGLBuffer vertexBuffer;
        QOpenGLVertexArrayObject vao;
        glm::vec3 color;
        glm::mat4 _modelMatrix;
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;
        std::string type;
};
