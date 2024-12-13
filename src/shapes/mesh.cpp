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

    // Буфер текстурных координат
    textureBuffer.create();
    textureBuffer.bind();
    textureBuffer.allocate(&texCoords[0], texCoords.size() * sizeof(glm::vec2));
    textureBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    normalBuffer.create();
    normalBuffer.bind();
    normalBuffer.allocate(&normals[0], normals.size() * sizeof(glm::vec3));
    normalBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(&vertices[0], vertices.size() * sizeof(glm::vec3));
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);


    // Инициализация буфера для индексов
    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(&indices[0], indices.size() * sizeof(unsigned int));


    // Получение функции OpenGL для дальнейшей работы с аттрибутами
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    
    // Настройка аттрибутов для текстурных координат (Location 2)
    f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    f->glEnableVertexAttribArray(2);

    // Настройка аттрибутов для нормалей (Location 1)
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    f->glEnableVertexAttribArray(0);

    // Настройка аттрибутов для вершин (Location 0)
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    f->glEnableVertexAttribArray(1);
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
    std::cout << indices.size() << std::endl;
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    vao.release();
    vertexBuffer.release();
    textureBuffer.release();
    normalBuffer.release();  // Не забудьте освободить normalBuffer
    indexBuffer.release();

    shaderProgram->release();
}
