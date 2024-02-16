#include "Cone.hpp"

Cone::Cone(glm::vec3 p, glm::vec2 c, float h, Material m)
    : Rendered(p, glm::vec3(0, 0, 0), m), c(c), h(h)
{
}

Cone::~Cone()
{
}
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/rotate_vector.hpp>

float Cone::distance(const glm::vec3 &p) const
{
    float q = glm::length(glm::vec2(p.x, p.z));
    return glm::max(glm::dot(c, glm::vec2(q, p.y)), -h - p.y);
}

glm::vec3 Cone::normal(const glm::vec3 &p) const
{
    // real normal
    glm::vec2 q = glm::vec2(p.x, p.z);
    glm::vec2 n = glm::normalize(glm::vec2(q.x * c.x, q.y));
    glm::vec3 normal = glm::vec3(n.x, -h / glm::length(c), n.y);
    return glm::normalize(normal);
}