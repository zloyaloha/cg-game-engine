#include "mesh.h"
#include <QOpenGLFunctions>
#include <iostream>

Mesh::Mesh(const glm::vec3 &position) : Shape(position, "mesh") {}

void Mesh::initialize() {
    shaderProgram->bind();
    _vao->bind();
    pos = shaderProgram->attributeLocation("aPos");
    _normal = shaderProgram->attributeLocation("aNormal");
    tex = shaderProgram->attributeLocation("aTexCoord");

    textureBuffer.create();
    textureBuffer.bind();
    textureBuffer.allocate(&texCoords[0], texCoords.size() * sizeof(glm::vec2));
    textureBuffer.setUsagePattern(QOpenGLBuffer::StreamDraw);
    shaderProgram->setAttributeBuffer(tex, GL_FLOAT, 0, 2);
    shaderProgram->enableAttributeArray(tex);

    normalBuffer.create();
    normalBuffer.bind();
    normalBuffer.allocate(&normals[0], normals.size() * sizeof(glm::vec3));
    normalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    shaderProgram->setAttributeBuffer(_normal, GL_FLOAT, 0, 3);
    shaderProgram->enableAttributeArray(_normal);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(&vertices[0], vertices.size() * sizeof(glm::vec3));
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    shaderProgram->setAttributeBuffer(pos, GL_FLOAT, 0, 3);
    shaderProgram->enableAttributeArray(pos);

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(&indices[0], indices.size() * sizeof(unsigned int));
    indexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _vao->release();
}

void Mesh::draw() {
    this->setMaterial(material);
    shaderProgram->bind();
    _vao->bind();

    vertexBuffer.bind();
    shaderProgram->setAttributeBuffer(pos, GL_FLOAT, 0, 3);
    shaderProgram->enableAttributeArray(pos);

    normalBuffer.bind();
    shaderProgram->setAttributeBuffer(_normal, GL_FLOAT, 0, 3);
    shaderProgram->enableAttributeArray(_normal);

    textureBuffer.bind();
    shaderProgram->setAttributeBuffer(tex, GL_FLOAT, 0, 2);
    shaderProgram->enableAttributeArray(tex);

    indexBuffer.bind();

    loadMatriciesToShader();
    loadLightsToShader();
    loadMaterialToShader();
    loadTextureToShader();

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    _vao->release();
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
