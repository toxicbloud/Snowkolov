#include "Sphere.hpp"
#include <iostream>

float multVec3(glm::vec3 a, glm::vec3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Sphere::Sphere(glm::vec3 p, float r)
    : Rendered(p, glm::vec3(0, 0, 0)), radius(r)
{

}

Sphere::~Sphere()
{

}

float Sphere::distance(const glm::vec3& p)
{
    return glm::length(p - position) - radius;
}

glm::vec3 Sphere::render()
{
    return glm::vec3(1.f, 0.1f, 0.f);
}
