#include "Noise.hpp"

float noise(float x)
{
    return glm::sin(2 * x) + glm::sin(PI * x);
}

glm::vec3 noise3D(glm::vec3 p, float scale, float strength, float seed)
{
    return glm::vec3(
        noise(p.x * scale + seed + 13.f) * strength,
        noise(p.y * scale + seed + 89.f) * strength,
        noise(p.z * scale + seed + 193.f) * strength
    );
}

Noise::Noise(const Rendered* target, float scale, float strength, float seed)
    : Rendered(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), target->getMaterial()),
      target(target), scale(scale), strength(strength), seed(seed)
{

}

Noise::~Noise()
{

}

float Noise::distance(const glm::vec3& p) const
{
    glm::vec3 noisedPoint = p + noise3D(p, scale, strength, seed);
    return target->getDistance(noisedPoint);
}

glm::vec3 Noise::normal(const glm::vec3& p) const
{
    glm::vec3 noisedPoint = p + noise3D(p, scale, strength, seed);
    return target->getNormal(noisedPoint);
}
