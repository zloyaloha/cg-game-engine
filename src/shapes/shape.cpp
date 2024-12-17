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

void Shape::loadMatriciesToShader()
{
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::vec3 cameraPosition = glm::vec3(glm::inverse(_viewMatrix)[3]);
    shaderProgram->setUniformValue("viewPos", QVector3D(cameraPosition.x, cameraPosition.y, cameraPosition.z));
    shaderProgram->setUniformValue("model", QMatrix4x4(glm::value_ptr(modelMatrix)).transposed());
    shaderProgram->setUniformValue("view", QMatrix4x4(glm::value_ptr(_viewMatrix)).transposed());
    shaderProgram->setUniformValue("projection", QMatrix4x4(glm::value_ptr(_projectionMatrix)).transposed());
}

void Shape::loadLightsToShader()
{
    shaderProgram->setUniformValue("numLights", int(lights.size()));
    std::string arrPos;
    for (int i = 0; i < lights.size(); ++i) {
        arrPos = "lights[" + std::to_string(i) + "].lightPos";
        shaderProgram->setUniformValue(arrPos.c_str(),
                                        QVector3D(lights[i]->position.x, lights[i]->position.y, lights[i]->position.z));
        arrPos = "lights[" + std::to_string(i) + "].lightColor";
        shaderProgram->setUniformValue(arrPos.c_str(),
                                        QVector3D(lights[i]->color.x, lights[i]->color.y, lights[i]->color.z));
        arrPos = "lights[" + std::to_string(i) + "].lightPointTo";
        shaderProgram->setUniformValue(arrPos.c_str(),
                                        QVector3D(lights[i]->pointTo.x, lights[i]->pointTo.y, lights[i]->pointTo.z));
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

void Shape::setModelMatrix(const glm::mat4 &modelMatrix) 
{
    _modelMatrix = modelMatrix;
}

void Shape::setProjectionMatrix(const glm::mat4 &projectionMatrix) 
{
    _projectionMatrix = projectionMatrix;
}
