#include "Noise.hpp"
#include "db_perlin.hpp"

float noise3D(float x, float y, float z, float scale, float seed)
{
    return db::perlin(x * scale + seed, y * scale + seed, z * scale + seed);
}

float getNoisedDistance(const Rendered* target, const glm::vec3& p, float scale, float strength, float seed)
{
    float noise = noise3D(p.x, p.y, p.z, scale, seed);
    return target->getDistance(p) + noise * strength;
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
    // returning distance without noise to save performance
    // (noise will be like a normal map, not modifying the model's shape)
    return target->getDistance(p);
}

glm::vec3 Noise::normal(const glm::vec3& p) const
{
    // calculating normal using finite differences
    // not the best performance-friendly way to do it, but it's the easiest
    float e = 0.01f;
    return glm::normalize(glm::vec3(
        getNoisedDistance(target, p + glm::vec3(e, 0, 0), scale, strength, seed) - getNoisedDistance(target, p - glm::vec3(e, 0, 0), scale, strength, seed),
        getNoisedDistance(target, p + glm::vec3(0, e, 0), scale, strength, seed) - getNoisedDistance(target, p - glm::vec3(0, e, 0), scale, strength, seed),
        getNoisedDistance(target, p + glm::vec3(0, 0, e), scale, strength, seed) - getNoisedDistance(target, p - glm::vec3(0, 0, e), scale, strength, seed)
    ));
}
