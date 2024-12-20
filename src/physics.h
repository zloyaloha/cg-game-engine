#pragma once
#include <vector>
#include <memory>
#include <shape.h>

class Physics {
    public:
        void calculateIntersect(const std::vector<std::shared_ptr<Shape>>& shapes) const;
        void resolveCollision(std::shared_ptr<Shape> shape1, std::shared_ptr<Shape> shape2) const;
};