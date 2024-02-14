#pragma once
#include "Rendered.hpp"

class SmoothUnion : public Rendered
{
private:
    Rendered *a, *b;
    float k;

public:
    SmoothUnion(Rendered *a, Rendered *b, float k, Material m);
    ~SmoothUnion();

    float distance(const glm::vec3 &p);
    glm::vec3 render(const glm::vec3 &p, const glm::vec3 &d);
    glm::vec3 normal(const glm::vec3 &p) const;
};
