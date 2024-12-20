#include "aabb.h"
#include <iostream>

void AABB::calculateAABB(const std::vector<glm::vec3> &vertices)
{
    min = vertices[0];
    max = vertices[0];
    for (const auto& vertex : vertices) {
        min.x = std::min(min.x, vertex.x);
        min.y = std::min(min.y, vertex.y);
        min.z = std::min(min.z, vertex.z);

        max.x = std::max(max.x, vertex.x);
        max.y = std::max(max.y, vertex.y);
        max.z = std::max(max.z, vertex.z);
    }
}

bool AABB::intersect(const AABB& other)
{
    if (max.x < other.min.x || min.x > other.max.x) return false;
    if (max.y < other.min.y || min.y > other.max.y) return false;
    if (max.z < other.min.z || min.z > other.max.z) return false;
    return true;
}

void AABB::translateAABB(const glm::vec3& translation)
{
    min += translation; 
    max += translation;
}

void AABB::scaleAABB(const glm::vec3& scale)
{
    glm::vec3 center = (min + max) / 2.0f;
    glm::vec3 halfSize = (max - min) / 2.0f;

    halfSize *= scale;

    min = center - halfSize;
    max = center + halfSize;
}

void AABB::rotateAABB(const glm::vec3& rotation)
{
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

    std::vector<glm::vec3> vertices = {
        min,
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        max,
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(max.x, max.y, min.z),
    };

    glm::vec3 newMin = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 newMax = glm::vec3(std::numeric_limits<float>::lowest());

    for (auto& vertex : vertices) {
        glm::vec3 transformedVertex = glm::vec3(rotationMatrix * glm::vec4(vertex, 1.0f));
        newMin = glm::min(newMin, transformedVertex);
        newMax = glm::max(newMax, transformedVertex);
    }

    min = newMin;
    max = newMax;
}

glm::vec3 AABB::getMin() const
{
    return min;
}

glm::vec3 AABB::getMax() const
{
    return max;
}

glm::vec3 &AABB::getMin()
{
    return min;
}

glm::vec3 &AABB::getMax()
{
    return max;
}
