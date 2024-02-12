#include "Material.hpp"

Material::Material(glm::vec3 color, float roughness, float metallic, float transparency)
    : color(color), roughness(roughness), metallic(metallic), transparency(transparency)
{
    
}

Material::~Material()
{

}

glm::vec3 Material::getColor() const
{
    return this->color;
}

float Material::getRoughness() const
{
    return this->roughness;
}

float Material::getMetallic() const
{
    return this->metallic;
}

float Material::getTransparency() const
{
    return this->transparency;
}
