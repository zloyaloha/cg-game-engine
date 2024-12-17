#include "mesh.h"
#include <QOpenGLFunctions>
#include <iostream>

Mesh::Mesh(const glm::vec3 &position, const glm::vec3& color) : Shape(position, color, "mesh") {}

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
    if (!shaderProgram->bind()) {
        qDebug() << "Failed to bind shader program.";
        return;
    }
    vao.bind();
    indexBuffer.bind();
    normalBuffer.bind();
    textureBuffer.bind();
    vertexBuffer.bind();

    this->loadMatriciesToShader();
    this->loadLightsToShader();
    this->loadObjectLightToShader();

    if (textures.size() != 0) {
        QOpenGLTexture *texture = textures[0].texture;
        texture->bind();
        // texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::MirroredRepeat);
        // texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::MirroredRepeat);
        // texture->setMinificationFilter(QOpenGLTexture::Linear);
        // texture->setMagnificationFilter(QOpenGLTexture::Linear);ис
        shaderProgram->setUniformValue("texture1", 0);
    }

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    vao.release();
    indexBuffer.release();
    normalBuffer.release();
    textureBuffer.release();
    vertexBuffer.release();
    shaderProgram->release();
}
