#pragma once

#include "shape.h"

class Cube : public Shape
{
public:
    Cube(float size, const glm::vec3& position);
    void initialize() override;
    void draw() override;
private:
    float size;
    void generateVertices();
};
