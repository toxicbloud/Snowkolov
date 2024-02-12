#pragma once
#include "Object.hpp"

#define MAXDIST std::numeric_limits<float>::max()

class Rendered: public Object
{
private:
    /* data */
public:
    Rendered(glm::vec3 p, glm::vec3 r);
    ~Rendered();
    virtual float distance(const glm::vec3& p) = NULL;
    virtual glm::vec3 render() = NULL;
};
