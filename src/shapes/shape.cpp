#include "shape.h"
#include "iostream"

Shape::Shape(const glm::vec3& shapePosition, const std::string& shapeType) : 
    vertexBuffer(QOpenGLBuffer::VertexBuffer), position(shapePosition), type(shapeType) {}

void Shape::setViewMatrix(const glm::mat4 &viewMatrix) 
{
    _viewMatrix = viewMatrix;
}

void Shape::setShader(std::shared_ptr<QOpenGLShaderProgram> shader)
{
    shaderProgram = shader;
}

void Shape::setMaterial(std::shared_ptr<Material> shapeMaterial)
{
    material = shapeMaterial;
}

void Shape::setScale(const glm::vec3& scale)
{
    _scale = scale;
    updateModelMatrix();
}

void Shape::setRotation(const glm::vec3& rotation)
{
    _rotation = rotation;
    updateModelMatrix();
}

void Shape::setPosition(const glm::vec3 &newPosition)
{
    _position = newPosition;
    updateModelMatrix();
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

glm::vec3 Shape::getPosition() const
{
    return position;
}

void Shape::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}


void Shape::setModelMatrix(const glm::mat4 &modelMatrix) 
{
    _modelMatrix = modelMatrix;
}

void Shape::setProjectionMatrix(const glm::mat4 &projectionMatrix) 
{
    _projectionMatrix = projectionMatrix;
}
