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
#include "light.h"

struct Material {
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
    float opacity;
};

struct Texture {
    QOpenGLTexture* texture;
    aiTextureType type;
};

class Shape {
    public:
        Shape(const glm::vec3& position, const std::string& type);
        virtual ~Shape() {}

        virtual void initialize() = 0;  // Абстрактный метод для инициализации данных
        virtual void draw() = 0;        // Абстрактный метод для рендеринга

        void setModelMatrix(const glm::mat4& modelMatrix);
        void setProjectionMatrix(const glm::mat4& projectionMatrix);
        void setViewMatrix(const glm::mat4& viewMatrix);
        void setShader(std::shared_ptr<QOpenGLShaderProgram> shader);
        void setVao(std::shared_ptr<QOpenGLVertexArrayObject> vao);
        void setMaterial(std::shared_ptr<Material> material);

        void setScale(const glm::vec3& scale);
        void setRotation(const glm::vec3& rotation);
        void setPosition(const glm::vec3 &newPosition);

        void updateModelMatrix();
        glm::vec3 getScale() const;
        glm::vec3 getRotation() const;
        glm::vec3 getPosition() const;
        
        void loadMatriciesToShader();
        void loadLightsToShader();
        void loadMaterialToShader();

        void setLights(const std::vector<std::shared_ptr<Light>>& light);
        std::string getType() const;
    protected:
        std::vector<std::shared_ptr<Light>> lights;
        std::shared_ptr<Material> material;

        glm::vec3 position;
        std::shared_ptr<QOpenGLShaderProgram> shaderProgram;
        std::shared_ptr<QOpenGLVertexArrayObject> _vao;
        glm::mat4 _modelMatrix;
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;

        glm::vec3 _scale;
        glm::vec3 _rotation;
        glm::vec3 _position;

        std::string type;

        QOpenGLBuffer vertexBuffer{QOpenGLBuffer::VertexBuffer};
        QOpenGLBuffer normalBuffer{QOpenGLBuffer::VertexBuffer};
        QOpenGLBuffer indexBuffer{QOpenGLBuffer::IndexBuffer};

        GLuint pos;
        GLuint normal;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
};
