#include "Rendered.hpp"

class Sphere: public Rendered
{
private:
    float radius;
public:
    Sphere(glm::vec3 p, float r, Material m);
    ~Sphere();

    float distance(const glm::vec3& p) const;
    glm::vec3 normal(const glm::vec3 &p) const;
};
