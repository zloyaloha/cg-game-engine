#pragma once

#include "shape.h"

class Cube : public Shape
{
public:
    Cube(float size, const glm::vec3& position);
    void initialize() override;
    void draw() override;
    void setModelMatrix(const glm::mat4& modelMatrix);
    void setProjectionMatrix(const glm::mat4& projectionMatrix);
    void setViewMatrix(const glm::mat4& viewMatrix);
private:
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    float size;
    glm::vec3 position;
    void generateVertices();
    void setPosition(const glm::vec3 &newPosition);
};
