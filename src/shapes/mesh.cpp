#include "mesh.h"
#include <QOpenGLFunctions>
#include <iostream>

Mesh::Mesh(const glm::vec3 &position, const glm::vec3& color) : Shape(position, color, "mesh") {}

bool Mesh::loadFromFile(const std::string& filepath) {
    ObjLoader loader;
    return loader.load(filepath, vertices, normals, texCoords, indices, textures);
}

void Mesh::initialize() {
    if (!vao.isCreated()) {
        vao.create();
    }
    vao.bind();

    shaderProgram->bind();
    pos = shaderProgram->attributeLocation("aPos");
    normal = shaderProgram->attributeLocation("aNormal");
    tex = shaderProgram->attributeLocation("aTexCoord");

    // Буфер текстурных координат
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
}

void Mesh::draw() {
    if (!shaderProgram->bind()) {
        qDebug() << "Failed to bind shader program.";
        return;
    }

    loadMatriciesToShader();
    loadLightsToShader();
    loadObjectLightToShader();

    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glActiveTexture(GL_TEXTURE0);
    if (textures.size() != 0) {
        f->glBindTexture(GL_TEXTURE_2D, textures[0].texture->textureId());
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
        shaderProgram->setUniformValue("texture1", 0);
    }

    vao.bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    vao.release();
    vertexBuffer.release();
    textureBuffer.release();
    normalBuffer.release();
    indexBuffer.release();

    shaderProgram->release();
}
