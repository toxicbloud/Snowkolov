#include "Rendered.hpp"

class Noise: public Rendered
{
private:
    const Rendered* target;
    float scale;
    float strength;
    float seed;
public:
    Noise(const Rendered* target, float scale, float strength, float seed);
    ~Noise();

    float distance(const glm::vec3& p) const;
    glm::vec3 normal(const glm::vec3 &p) const;
};
