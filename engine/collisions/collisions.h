#pragma once

#include "../graphics/graphics.h"

extern std::vector<TriAABB> colliders;

void resolveCollisions(Capsule* cap, float deltaTime);

void collectSceneColliders();