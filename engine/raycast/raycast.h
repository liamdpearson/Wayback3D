#pragma once

#include "../graphics/graphics.h"

#include <glm/glm.hpp>

#include <vector>
#include <memory>

struct BVHnode
{
    AABB aabb;
    std::unique_ptr<BVHnode> left;
    std::unique_ptr<BVHnode> right;
    std::vector<Tri*> tris;
    bool isLeaf = false;
};

bool rayOccluded(const glm::vec3& origin, const glm::vec3& dir,
                        float maxDist, const BVHnode& node);