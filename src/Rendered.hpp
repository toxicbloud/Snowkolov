#pragma once
#include "Object.hpp"
#include "Material.hpp"

#define MAXDIST std::numeric_limits<float>::max()

class Rendered: public Object
{
protected:
    float getLightAmount(const glm::vec3& p, const glm::vec3& d);
    Material material;

    virtual float distance(const glm::vec3& p) = NULL;
    virtual glm::vec3 normal(const glm::vec3& p) const = NULL;

public:
    Rendered(glm::vec3 p, glm::vec3 r, Material m);
    ~Rendered();

    float getDistance(const glm::vec3& p);
    glm::vec3 render(const glm::vec3& p, const glm::vec3& d);

    Material getMaterial() const;
};
