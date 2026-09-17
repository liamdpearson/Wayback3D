#pragma once

#include "../graphics/graphics.h"
#include "../raycast/raycast.h"

#include <glm/glm.hpp>

#include <vector>


extern std::vector<Tri> occluders;
extern BVHnode rootNode;

glm::vec3 sampleLightAt(const glm::vec3& p);

void bakeSceneLighting();