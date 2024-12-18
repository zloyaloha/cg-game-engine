#pragma once

#include "shape.h"

class Cube : public Shape
{
public:
    Cube(float size, const glm::vec3& position);
    void initialize() override;
    void draw() override;
private:
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    float size;
    void generateVertices();
    void setPosition(const glm::vec3 &newPosition);
};
