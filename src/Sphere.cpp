#include "Sphere.hpp"

Sphere::Sphere(glm::vec3 p, float r, Material m)
    : Rendered(p, glm::vec3(0, 0, 0), m), radius(r)
{

}

Sphere::~Sphere()
{

}

float Sphere::distance(const glm::vec3& p)
{
    return glm::length(p - position) - radius;
}

glm::vec3 Sphere::normal(const glm::vec3& p) const
{
    return glm::normalize(p - position);
}
