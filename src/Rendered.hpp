#pragma once
#include "Object.hpp"
#include "Material.hpp"

#define MAXDIST std::numeric_limits<float>::max()

class Rendered: public Object
{
protected:
    float getLightAmount(const glm::vec3& p, const glm::vec3& d);
    Material material;

public:
    Rendered(glm::vec3 p, glm::vec3 r, Material m);
    ~Rendered();
    virtual float distance(const glm::vec3& p) = NULL;
    glm::vec3 render(const glm::vec3& p, const glm::vec3& d);

    Material getMaterial() const;
};
