#include "physics.h"
#include <iostream>

void Physics::calculateIntersect(const std::vector<std::shared_ptr<Shape>>& shapes) const
{
    for (int i = 0; i < shapes.size(); ++i) {
        for (int j = i + 1; j < shapes.size(); ++j) {
            if (shapes[i]->calculateIntersect(shapes[j])) {
                if (!shapes[i]->isGhost() && !shapes[j]->isGhost()) {
                    resolveCollision(shapes[i], shapes[j]);
                }
            }
        }
    }
}

void Physics::resolveCollision(std::shared_ptr<Shape> shape1, std::shared_ptr<Shape> shape2) const
{
    std::shared_ptr<AABB> aAABB = shape1->getAABB();
    std::shared_ptr<AABB> bAABB = shape2->getAABB();

    const float epsilon = 0.1;

    glm::vec3 overlap(
        std::min(aAABB->getMax().x - bAABB->getMin().x, bAABB->getMax().x - aAABB->getMin().x),
        std::min(aAABB->getMax().y - bAABB->getMin().y, bAABB->getMax().y - aAABB->getMin().y),
        std::min(aAABB->getMax().z - bAABB->getMin().z, bAABB->getMax().z - aAABB->getMin().z)
    );

    if (overlap.x < epsilon && overlap.y < epsilon && overlap.z < epsilon) {
        return;
    }

    float minOverlap = std::min(overlap.x, std::min(overlap.y, overlap.z));

    glm::vec3 resolution;
    if (minOverlap == overlap.x) {
        resolution = glm::vec3(overlap.x, 0.0f, 0.0f);
    } else if (minOverlap == overlap.y) {
        resolution = glm::vec3(0.0f, overlap.y, 0.0f);
    } else {
        resolution = glm::vec3(0.0f, 0.0f, overlap.z);
    }

    glm::vec3 directionA = shape1->getPosition() - shape2->getPosition();
    glm::vec3 directionB = shape2->getPosition() - shape1->getPosition();

    float massA = shape1->getMass();
    float massB = shape2->getMass();

    glm::vec3 velocityA = shape1->getVelocity();
    glm::vec3 velocityB = shape2->getVelocity();

    glm::vec3 newVelocityA = (velocityA * (massA - massB) + 2.0f * massB * velocityB) / (massA + massB);
    glm::vec3 newVelocityB = (velocityB * (massB - massA) + 2.0f * massA * velocityA) / (massA + massB);

    shape1->setVelocity(newVelocityA);
    shape2->setVelocity(newVelocityB);

    glm::vec3 newPositionA = shape1->getPosition() + (directionA.x > 0.0f ? resolution : -resolution) * 0.5f;
    glm::vec3 newPositionB = shape2->getPosition() + (directionB.x > 0.0f ? resolution : -resolution) * 0.5f;

    shape1->setPosition(newPositionA);
    shape2->setPosition(newPositionB);
}




