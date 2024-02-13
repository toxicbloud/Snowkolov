#include "Sphere.hpp"

float multVec3(glm::vec3 a, glm::vec3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

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
