#pragma once

#include "../graphics/graphics.h"
#include "../raycast/raycast.h"

#include <glm/glm.hpp>

#include <vector>
#include <utility>


// scene light used only in lighting bake
struct Light
{
    std::string name;
    std::string tag;
    glm::vec3 pos;
    glm::vec3 color;
    float intensity;
    float radius;
    float falloff;
};

struct LightGrid
{
    std::vector<std::pair<glm::vec3, glm::vec3>> values;
    glm::vec3 min{INFINITY}, max{-INFINITY};
};

extern std::vector<Light> lights;
extern float ambient;
extern float lightmapResScalar;
extern std::vector<Tri> occluders;
extern BVHnode rootNode;
extern LightGrid lightGrid;

// for finding the bounds box of the scene for light grid
extern float minX;
extern float maxX;
extern float minY;
extern float maxY;
extern float minZ;
extern float maxZ;

glm::vec3 sampleLightAt(const glm::vec3& p);

std::pair<glm::vec3, glm::vec3> gridLightAt(const glm::vec3& p);

void bakeSceneLighting();