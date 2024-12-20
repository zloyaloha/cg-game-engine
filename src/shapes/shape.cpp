#include "shape.h"
#include "iostream"

Shape::Shape(const glm::vec3& shapePosition, const std::string& shapeType) : 
    position(shapePosition), type(shapeType), _aabb(std::make_shared<AABB>()) {}

void Shape::setViewMatrix(const glm::mat4 &viewMatrix) 
{
    _viewMatrix = viewMatrix;
}

void Shape::setShader(std::shared_ptr<QOpenGLShaderProgram> shader)
{
    shaderProgram = shader;
}

void Shape::setVao(std::shared_ptr<QOpenGLVertexArrayObject> vao)
{
    _vao = vao;
}

void Shape::setMaterial(std::shared_ptr<Material> shapeMaterial)
{
    material = shapeMaterial;
}

void Shape::setScale(const glm::vec3& scale)
{
    _scale = scale;
    _aabb->scaleAABB(scale);
    updateModelMatrix();
}

void Shape::setRotation(const glm::vec3& rotation)
{
    _rotation = rotation;
    _aabb->rotateAABB(rotation);
    updateModelMatrix();
}

void Shape::setPosition(const glm::vec3 &position)
{
    _aabb->translateAABB(position - _position);
    _position = position;
    updateModelMatrix();
}

void Shape::setVelocity(const glm::vec3 &velocity)
{
    _velocity = velocity;
}

void Shape::setAcceleration(const glm::vec3 &acceleration)
{
    _acceleration = acceleration;
}

void Shape::update(float deltaTime)
{
    _velocity += _acceleration * deltaTime;
    setPosition(_position + _velocity * deltaTime);

    if (_position.y < 0.0f) {
        _position.y = 0.0f;
        _velocity.y = 0.0f;
    }
}

void Shape::updateModelMatrix() {
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), _position);

    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), _scale);

    glm::mat4 modelMatrix = translationMatrix * rotationZ * rotationY * rotationX * scaleMatrix;

    _modelMatrix = modelMatrix;
}

glm::vec3 Shape::getScale() const
{
    return _scale;
}

glm::vec3 Shape::getRotation() const
{
    return _rotation;
}

glm::vec3 Shape::getPosition() const
{
    return _position;
}

glm::vec3 Shape::getVelocity() const
{
    return _velocity;
}

glm::vec3 Shape::getAcceleration() const
{
    return _acceleration;
}

std::shared_ptr<AABB> Shape::getAABB() const
{
    return _aabb;
}

void Shape::loadMatriciesToShader()
{
    glm::vec3 cameraPosition = glm::vec3(glm::inverse(_viewMatrix)[3]);
    shaderProgram->setUniformValue("viewPos", QVector3D(cameraPosition.x, cameraPosition.y, cameraPosition.z));
    shaderProgram->setUniformValue("model", QMatrix4x4(glm::value_ptr(_modelMatrix)).transposed());
    shaderProgram->setUniformValue("view", QMatrix4x4(glm::value_ptr(_viewMatrix)).transposed());
    shaderProgram->setUniformValue("projection", QMatrix4x4(glm::value_ptr(_projectionMatrix)).transposed());
}

void Shape::loadLightsToShader()
{
    int numPointLights = 0;
    int numSpotLights = 0;
    int numDirLights = 0;

    for (int i = 0; i < lights.size(); ++i) {
        Light::LightType type = lights[i]->getType();
        if (type == Light::POINT) {
            ++numPointLights;
        } else if (type == Light::SPOT) {
            ++numSpotLights;
        } else if (type == Light::DIRECTIONAL) {
            ++numDirLights;
        }
    }

    shaderProgram->setUniformValue("numPointLights", numPointLights);
    shaderProgram->setUniformValue("numSpotLights", numSpotLights);
    shaderProgram->setUniformValue("numDirLights", numDirLights);

    int pointLightIndex = 0;
    int spotLightIndex = 0;
    int dirLightIndex = 0;

    for (int i = 0; i < lights.size(); ++i) {
        Light::LightType type = lights[i]->getType();
        if (type == Light::POINT) {
            lights[i]->loadLightToShader(shaderProgram, pointLightIndex++);
        } else if (type == Light::SPOT) {
            lights[i]->loadLightToShader(shaderProgram, spotLightIndex++);
        } else if (type == Light::DIRECTIONAL) {
            lights[i]->loadLightToShader(shaderProgram, dirLightIndex++);
        }
    }
}

void Shape::loadMaterialToShader()
{
    shaderProgram->setUniformValue("material.ambientColor", QVector3D(material->ambientColor.x, material->ambientColor.y, material->ambientColor.z));
    shaderProgram->setUniformValue("material.diffuseColor", QVector3D(material->diffuseColor.x, material->diffuseColor.y, material->diffuseColor.z));
    shaderProgram->setUniformValue("material.specularColor", QVector3D(material->specularColor.x, material->specularColor.y, material->specularColor.z));
    shaderProgram->setUniformValue("material.shininess", material->shininess);
}

void Shape::setLights(const std::vector<std::shared_ptr<Light>> &shapeLights)
{
    lights = shapeLights;
}

std::string Shape::getType() const
{
    return type;
}

void Shape::initAABB()
{
    _aabb->calculateAABB(vertices);
}

bool Shape::calculateIntersect(std::shared_ptr<Shape> other) const
{
    return _aabb->intersect(*other->getAABB());
}

void Shape::setModelMatrix(const glm::mat4 &modelMatrix) 
{
    _modelMatrix = modelMatrix;
}

void Shape::setProjectionMatrix(const glm::mat4 &projectionMatrix) 
{
    _projectionMatrix = projectionMatrix;
}
