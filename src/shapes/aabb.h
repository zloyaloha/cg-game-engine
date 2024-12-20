#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

class AABB {
    public:
        void calculateAABB(const std::vector<glm::vec3>& vertices);
        bool intersect(const AABB& other);
        void translateAABB(const glm::vec3 &translation);
        void scaleAABB(const glm::vec3 &scale);
        void rotateAABB(const glm::vec3& rotation);
    private:
        glm::vec3 min;
        glm::vec3 max; 
};