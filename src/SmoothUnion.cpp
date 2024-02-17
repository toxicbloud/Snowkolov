#include "SmoothUnion.hpp"

SmoothUnion::SmoothUnion(Rendered *a, Rendered *b, float k, Material m)
    : Rendered(a->getPosition(), glm::vec3(0, 0, 0), m), a(a), b(b), k(k)
{
}

float SmoothUnion::distance(const glm::vec3 &p) const
{
    float h = (float)glm::clamp(0.5 + 0.5 * (this->b->getDistance(p) - this->a->getDistance(p)) / this->k, 0.0, 1.0);
    return glm::mix(this->b->getDistance(p), this->a->getDistance(p), h) - this->k * h * (1.0f - h);
}

glm::vec3 SmoothUnion::normal(const glm::vec3 &p) const
{
    float h = (float)glm::clamp(0.5 + 0.5 * (this->b->getDistance(p) - this->a->getDistance(p)) / this->k, 0.0, 1.0);
    return glm::normalize(glm::mix(this->b->getNormal(p), this->a->getNormal(p), h));
}
