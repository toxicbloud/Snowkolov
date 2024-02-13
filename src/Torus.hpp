#include "Rendered.hpp"

class Torus : public Rendered
{
private:
    float radius;
    float thickness;

public:
    Torus(glm::vec3 p, float r, float t, Material m);
    ~Torus();

    float distance(const glm::vec3 &p);
    glm::vec3 render(const glm::vec3 &p, const glm::vec3 &d);
    glm::vec3 normal(const glm::vec3 &p) const;
};
