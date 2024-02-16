#pragma once
#include "Rendered.hpp"

class Cone : public Rendered
{
private:
    const glm::vec2 c;
    const float h;

public:
    Cone(glm::vec3 p, glm::vec2 c, float h, Material m);
    ~Cone();

    float distance(const glm::vec3 &p) const;
    glm::vec3 render(const glm::vec3 &p, const glm::vec3 &d);
    glm::vec3 normal(const glm::vec3 &p) const;
};
