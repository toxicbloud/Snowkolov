#include "Rendered.hpp"
#include <utility>

Rendered::Rendered(glm::vec3 p, glm::vec3 r, Material m)
    : Object(p, r), material(m)
{

}

Rendered::~Rendered()
{

}

float Rendered::getLightAmount(const glm::vec3& p, const glm::vec3& d)
{
    float ambient = 0.3f;
    float diffuse = material.getRoughness();
    float specular = 1.0f - material.getRoughness();

    glm::vec3 n = this->normal(p);

    glm::vec3 lightDir = glm::normalize(glm::vec3(0, 0, -1));
    float diff = std::max(glm::dot(n, lightDir), 0.0f);
    float spec = (float) glm::pow(diff, 256 - material.getRoughness() * 256);

    return std::min(std::max(ambient + (diffuse * diff + specular * spec) * (1.f - ambient), 0.0f), 1.0f);
}

glm::vec3 Rendered::render(const glm::vec3& p, const glm::vec3& d)
{
    float light = getLightAmount(p, d);
    return material.getColor() * light;
}

Material Rendered::getMaterial() const
{
    return material;
}