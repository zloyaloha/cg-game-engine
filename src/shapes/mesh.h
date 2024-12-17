#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <vector>
#include <string>
#include <QDebug>
#include "shape.h"

class Mesh : public Shape {
public:
    friend class ObjLoader;
    Mesh(const glm::vec3 &position, const glm::vec3& color);

    void initialize() override;

    void draw() override;

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<Texture> textures;
    std::vector<unsigned int> indices;

    QOpenGLBuffer vertexBuffer{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer normalBuffer{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer textureBuffer{QOpenGLBuffer::VertexBuffer};
    QOpenGLBuffer indexBuffer{QOpenGLBuffer::IndexBuffer};

    GLuint pos;
    GLuint normal;
    GLuint tex;

    QOpenGLVertexArrayObject vao;
};
