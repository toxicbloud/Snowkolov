#pragma once
#include "Rendered.hpp"

class Cone : public Rendered
{
private:
    float radius;
    float height;

public:
    Cone(glm::vec3 p, glm::vec3 r, float d, float h, Material m);
    ~Cone();

    float distance(const glm::vec3 &p) const;
    glm::vec3 render(const glm::vec3 &p, const glm::vec3 &d);
    glm::vec3 normal(const glm::vec3 &p) const;
};
