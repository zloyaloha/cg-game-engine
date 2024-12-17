#include "mesh.h"
#include <QOpenGLFunctions>
#include <iostream>

Mesh::Mesh(const glm::vec3 &position) : Shape(position, "mesh") {}

void Mesh::initialize() {
    if (!vao.isCreated()) {
        vao.create();
    }
    vao.bind();

    shaderProgram->bind();
    pos = shaderProgram->attributeLocation("aPos");
    normal = shaderProgram->attributeLocation("aNormal");
    tex = shaderProgram->attributeLocation("aTexCoord");

    textureBuffer.create();
    textureBuffer.bind();
    textureBuffer.allocate(&texCoords[0], texCoords.size() * sizeof(glm::vec2));
    textureBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    shaderProgram->enableAttributeArray(tex);
    shaderProgram->setAttributeBuffer(tex, GL_FLOAT, 0, 2);

    normalBuffer.create();
    normalBuffer.bind();
    normalBuffer.allocate(&normals[0], normals.size() * sizeof(glm::vec3));
    normalBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    shaderProgram->enableAttributeArray(normal);
    shaderProgram->setAttributeBuffer(normal, GL_FLOAT, 0, 3);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(&vertices[0], vertices.size() * sizeof(glm::vec3));
    vertexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    shaderProgram->enableAttributeArray(pos);
    shaderProgram->setAttributeBuffer(pos, GL_FLOAT, 0, 3);

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(&indices[0], indices.size() * sizeof(unsigned int));
    indexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    vao.release();
}

void Mesh::draw() {
    // std::shared_ptr<Material> material = std::make_shared<Material>();
    // material->ambientColor = glm::vec3(0.1f, 0.1f, 0.1f);  // Тусклый амбиентный цвет
    // material->diffuseColor = glm::vec3(0.6f, 0.6f, 0.6f);  // Тусклый диффузный цвет
    // material->specularColor = glm::vec3(1.0f, 1.0f, 1.0f);  // Нет зеркальных отражений (матовый)
    // material->shininess = 10.0f;  // Шершавость поверхности, меньше — более матовая
    this->setMaterial(material);
    if (!shaderProgram->bind()) {
        qDebug() << "Failed to bind shader program.";
        return;
    }
    vao.bind();
    indexBuffer.bind();
    normalBuffer.bind();
    textureBuffer.bind();
    vertexBuffer.bind();

    loadMatriciesToShader();
    loadLightsToShader();
    loadMaterialToShader();
    loadTextureToShader();

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    vao.release();
    indexBuffer.release();
    normalBuffer.release();
    textureBuffer.release();
    vertexBuffer.release();
    shaderProgram->release();
}

void Mesh::loadTextureToShader()
{
    bool useDiffuse = false;
    bool useSpecular = false;
    int i = 0;
    for (const auto& texture : textures) {
        if (texture.type == aiTextureType_DIFFUSE) {
            glActiveTexture(GL_TEXTURE0);
            texture.texture->bind();
            shaderProgram->setUniformValue("diffuseTexture", 0);
            useDiffuse = true;
        } else if (texture.type == aiTextureType_SPECULAR) {
            glActiveTexture(GL_TEXTURE1);
            texture.texture->bind();
            shaderProgram->setUniformValue("specularTexture", 1);
            useSpecular = true;
        }
    }

    shaderProgram->setUniformValue("useDiffuseTexture", useDiffuse);
    shaderProgram->setUniformValue("useSpecularTexture", useSpecular);
}
