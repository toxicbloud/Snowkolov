#include "Rendered.hpp"

class Capsule : public Rendered
{
private:
    float radius;
    float height;

public:
    Capsule(glm::vec3 p, float r, float height, Material m);
    ~Capsule();

    float distance(const glm::vec3 &p);
    glm::vec3 render(const glm::vec3 &p, const glm::vec3 &d);
};
