#pragma once
#include <vector>
#include "Rendered.hpp"

class Scene
{
private:
    std::vector<Rendered*> objects;
public:
    Scene();
    ~Scene();
    void addObject(Rendered* o);
    std::vector<Rendered*> getObjects() const;
    glm::vec3 normal(const glm::vec3& p) const;
};
