#include "shape.h"
#include "iostream"

Shape::Shape(const glm::vec3& shapePosition, const glm::vec3& shapeColor, const std::string& shapeType) : 
    vertexBuffer(QOpenGLBuffer::VertexBuffer), color(shapeColor), position(shapePosition), type(shapeType) {}

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
    color = shapeColor;
}

void Shape::loadMatriciesToShader()
{
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    shaderProgram->setUniformValue("model", QMatrix4x4(glm::value_ptr(modelMatrix)).transposed());
    shaderProgram->setUniformValue("view", QMatrix4x4(glm::value_ptr(_viewMatrix)).transposed());
    shaderProgram->setUniformValue("projection", QMatrix4x4(glm::value_ptr(_projectionMatrix)).transposed());
}

void Shape::loadLightsToShader()
{
    shaderProgram->setUniformValue("numLights", int(lights.size()));
    for (int i = 0; i < lights.size(); ++i) {
        shaderProgram->setUniformValue(("lightPos[" + std::to_string(i) + "]").c_str(),
                                        QVector3D(lights[i]->position.x, lights[i]->position.y, lights[i]->position.z));
        shaderProgram->setUniformValue(("lightColor[" + std::to_string(i) + "]").c_str(),
                                        QVector3D(lights[i]->color.x, lights[i]->color.y, lights[i]->color.z));
        shaderProgram->setUniformValue(("lightPointTo[" + std::to_string(i) + "]").c_str(),
                                        QVector3D(lights[i]->pointTo.x, lights[i]->pointTo.y, lights[i]->pointTo.z));
    }
}

void Shape::loadObjectLightToShader()
{
    shaderProgram->setUniformValue("objectColor", QVector3D(color.r, color.g, color.b));
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
