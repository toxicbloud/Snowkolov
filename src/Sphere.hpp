#include "Rendered.hpp"

class Sphere: public Rendered
{
private:
    float radius;
public:
    Sphere(glm::vec3 p, float r);
    ~Sphere();

    float distance(const glm::vec3& p);
    glm::vec3 render();
};
