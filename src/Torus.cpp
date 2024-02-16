#include "Torus.hpp"
#include <utility>
#include <iostream>

Torus::Torus(glm::vec3 p, float r, float t, Material m)
    : Rendered(p, glm::vec3(0, 0, 0), m), radius(r), thickness(t)
{
}

Torus::~Torus()
{
}

float Torus::distance(const glm::vec3 &p) const
{
    glm::vec2 q = glm::vec2(glm::length(glm::vec2(p.x - position.x, p.z - position.z)) - radius, p.y - position.y);
    return glm::length(q) - thickness;
}

glm::vec3 Torus::normal(const glm::vec3 &p) const
{
    glm::vec2 q = glm::vec2(glm::length(glm::vec2(p.x - position.x, p.z - position.z)) - radius, p.y - position.y);
    glm::vec3 n = glm::normalize(glm::vec3(
        2.0f * q.x * (p.x - position.x),
        2.0f * q.y,
        2.0f * q.x * (p.z - position.z)
    ));
    return n;
}
