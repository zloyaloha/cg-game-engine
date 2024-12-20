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
#include <list>
#include "aabb.h"


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

class ShapeMemento;
class ShapeHistory;

class Shape : public std::enable_shared_from_this<Shape> {
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
        void setVelocity(const glm::vec3& velocity);
        void setAcceleration(const glm::vec3 &acceleration);
        void setInitialPosition(const glm::vec3& position);
        void setMass(float mass);
        void changeGhostRegime();

        void updateModelMatrix();
        glm::vec3 getScale() const;
        glm::vec3 getRotation() const;
        glm::vec3 getPosition() const;

        
        void setAmbientColor(const glm::vec3& color);
        void setDiffuseColor(const glm::vec3& color);
        void setSpecularColor(const glm::vec3& color);
        void setShininess(const float value);

        glm::vec3 getAmbientColor();
        glm::vec3 getDiffuseColor();
        glm::vec3 getSpecularColor();
        float getShininess();

        glm::vec3 getInitialPosition() const;
        glm::vec3 getVelocity() const;
        glm::vec3 getAcceleration() const;
        std::shared_ptr<AABB> getAABB() const;
        float getMass() const;
        bool isGhost() const;
        std::string getType() const;

        std::shared_ptr<ShapeMemento> createMemento() const;
        void restoreFromMemento(const std::shared_ptr<ShapeMemento>& memento);
        void saveState();
        void restoreState();


        void loadMatriciesToShader();
        void loadLightsToShader();
        void loadMaterialToShader();

        void setLights(const std::vector<std::shared_ptr<Light>>& light);

        void update(float deltaTime);
        void initAABB();
        bool calculateIntersect(std::shared_ptr<Shape> other) const;
    protected:
        std::vector<std::shared_ptr<Light>> lights;
        std::shared_ptr<Material> material;

        std::shared_ptr<QOpenGLShaderProgram> shaderProgram;
        std::shared_ptr<QOpenGLVertexArrayObject> _vao;
        glm::mat4 _modelMatrix;
        glm::mat4 _projectionMatrix;
        glm::mat4 _viewMatrix;

        glm::vec3 _scale;
        glm::vec3 _rotation;
        glm::vec3 _position;
        float _mass;

        std::string type;

        QOpenGLBuffer vertexBuffer{QOpenGLBuffer::VertexBuffer};
        QOpenGLBuffer normalBuffer{QOpenGLBuffer::VertexBuffer};
        QOpenGLBuffer indexBuffer{QOpenGLBuffer::IndexBuffer};

        GLuint pos;
        GLuint _normal;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        std::shared_ptr<AABB> _aabb;
        bool _isGhost{true};

        std::shared_ptr<ShapeHistory> history;
    protected:
        glm::vec3 _velocity;
        glm::vec3 _acceleration;
        glm::vec3 _initialPosition;
};

class ShapeMemento {
public:
    ShapeMemento(const glm::vec3& position, const glm::vec3& velocity, const glm::vec3& acceleration,
                 const glm::vec3& scale, const glm::vec3& rotation, float mass,
                 const std::string& type, bool isGhost)
        : position(position), velocity(velocity), acceleration(acceleration),
          scale(scale), rotation(rotation), mass(mass), type(type), isGhost(isGhost) {}

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getVelocity() const { return velocity; }
    glm::vec3 getAcceleration() const { return acceleration; }
    glm::vec3 getScale() const { return scale; }
    glm::vec3 getRotation() const { return rotation; }
    float getMass() const { return mass; }
    std::string getType() const { return type; }
    bool getIsGhost() const { return isGhost; }

private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 scale;
    glm::vec3 rotation;
    float mass;
    std::string type;
    bool isGhost;
};

class ShapeHistory {
public:
    void saveState(const std::shared_ptr<Shape> shape);

    void restoreState(std::shared_ptr<Shape> shape);
private:
    std::list<std::shared_ptr<ShapeMemento>> history;
};
