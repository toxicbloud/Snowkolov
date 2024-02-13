#include "Capsule.hpp"
#include <utility>

Capsule::Capsule(glm::vec3 p1, glm::vec3 p2, float t, Material m)
    : Rendered(p1, glm::vec3(0, 0, 0), m), position2(p2), thickness(t)
{
}

Capsule::~Capsule()
{
}

float Capsule::distance(const glm::vec3 &p)
{
    glm::vec3 pa = p - this->position,
              ba = this->position2 - this->position;

    float h = std::min(std::max(dot(pa,ba) / dot(ba,ba), 0.f), 1.f);
    return length( pa - ba*h ) - thickness;
}

glm::vec3 Capsule::normal(const glm::vec3 &p) const
{
    glm::vec3 pa = p - this->position,
              ba = this->position2 - this->position;

    float h = std::min(std::max(dot(pa,ba) / dot(ba,ba), 0.f), 1.f);
    glm::vec3 n = normalize(pa - ba*h);
    return n;
}
