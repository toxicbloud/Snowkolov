#include "Rendered.hpp"

class Capsule : public Rendered
{
private:
    float thickness;
    glm::vec3 position2;

public:
    Capsule(glm::vec3 p1, glm::vec3 p2, float t, Material m);
    ~Capsule();

    float distance(const glm::vec3 &p) const;
    glm::vec3 render(const glm::vec3 &p, const glm::vec3 &d);
    glm::vec3 normal(const glm::vec3 &p) const;
};
