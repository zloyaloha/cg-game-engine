#include "light.h"

void PointLight::loadLightToShader(std::shared_ptr<QOpenGLShaderProgram> shader, int index) const
{
    shader->setUniformValue(("pointLights[" + std::to_string(index) + "].lightPos").c_str(), QVector3D(position.x, position.y, position.z));
    shader->setUniformValue(("pointLights[" + std::to_string(index) + "].lightColor").c_str(), QVector3D(color.x, color.y, color.z));
    shader->setUniformValue(("pointLights[" + std::to_string(index) + "].intensity").c_str(), intensity);
    shader->setUniformValue(("pointLights[" + std::to_string(index) + "].constant").c_str(), constant);
    shader->setUniformValue(("pointLights[" + std::to_string(index) + "].linear").c_str(), linear);
    shader->setUniformValue(("pointLights[" + std::to_string(index) + "].quadratic").c_str(), quadratic);
}

void SpotLight::loadLightToShader(std::shared_ptr<QOpenGLShaderProgram> shader, int index) const
{
    shader->setUniformValue(("spotLights[" + std::to_string(index) + "].lightPos").c_str(), QVector3D(position.x, position.y, position.z));
    shader->setUniformValue(("spotLights[" + std::to_string(index) + "].lightDir").c_str(), QVector3D(direction.x, direction.y, direction.z));
    shader->setUniformValue(("spotLights[" + std::to_string(index) + "].lightColor").c_str(), QVector3D(color.x, color.y, color.z));
    shader->setUniformValue(("spotLights[" + std::to_string(index) + "].intensity").c_str(), intensity);
    shader->setUniformValue(("spotLights[" + std::to_string(index) + "].cutOff").c_str(), cutOff);
    shader->setUniformValue(("spotLights[" + std::to_string(index) + "].outerCutOff").c_str(), outerCutOff);
}

void DirectionalLight::loadLightToShader(std::shared_ptr<QOpenGLShaderProgram> shader, int index) const
{
    shader->setUniformValue(("dirLights[" + std::to_string(index) + "].lightDir").c_str(), QVector3D(direction.x, direction.y, direction.z));
    shader->setUniformValue(("dirLights[" + std::to_string(index) + "].lightColor").c_str(), QVector3D(color.x, color.y, color.z));
    shader->setUniformValue(("dirLights[" + std::to_string(index) + "].intensity").c_str(), intensity);
}
