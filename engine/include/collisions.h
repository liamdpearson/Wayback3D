#pragma once

#include "graphics.h"
#include "raycast.h"


class Capsule : public Object
{
     public:
        float height = 1.8f; // capsule height
        float radius = 0.3f; // capsule width
        glm::vec3 velocity{0.0f};

        Capsule() = default;

        Capsule(const Transform t,
               const float height, const float radius)
        : height(height), radius(radius) {
            transform = t;
        }

        void Compose() override;

        ~Capsule() = default;
};

extern std::vector<TriAABB> colliders;
extern BVHnode rootNode;

void resolveCollisions(Capsule* cap, float deltaTime);

void collectSceneColliders();
