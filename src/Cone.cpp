#include "Cone.hpp"

Cone::Cone(glm::vec3 p, glm::vec3 r, float d, float h, Material m)
    : Rendered(p, r, m), radius(d), height(h)
{
}

Cone::~Cone()
{
}

float Cone::distance(const glm::vec3 &p) const
{
    glm::vec2 p2d = glm::vec2(p.x - position.x, p.z - position.z);
    float hr = (p.y - position.y) / height;
    float inf = glm::max(glm::length(p2d) - (1-hr) * radius, 0.f);
    return glm::max(inf, -hr);
}

glm::vec3 Cone::normal(const glm::vec3 &p) const
{
    glm::vec2 p2d = glm::vec2(p.x - position.x, p.z - position.z);
    float baseAngle = glm::atan(p2d.y, p2d.x);
    float heightAngle = glm::atan(height, radius);
    return glm::normalize(glm::vec3(
        glm::cos(baseAngle) * glm::cos(heightAngle),
        glm::sin(heightAngle),
        glm::sin(baseAngle) * glm::cos(heightAngle)
    ));
}
