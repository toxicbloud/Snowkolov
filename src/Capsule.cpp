#include "Capsule.hpp"

Capsule::Sphere(glm::vec3 p, float r, float h, Material m)
    : Rendered(p, glm::vec3(0, 0, 0), m), radius(r), height(h)
{
}

Capsule::~Capsule()
{
}

float Capsule::distance(const glm::vec3 &p)
{
    glm::vec3 adjustedP = p;
    adjustedP.y -= glm::clamp(p.y, 0.0f, this->height);
    return glm::length(adjustedP) - this->radius;
}
