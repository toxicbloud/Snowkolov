#pragma once
#include <glm/glm.hpp>

class Material
{
private:
    glm::vec3 color;
    float roughness;
    float metallic;
    float transparency;

public:
    Material(glm::vec3 color, float roughness, float metallic, float transparency);
    ~Material();

    glm::vec3 getColor() const;
    float getRoughness() const;
    float getMetallic() const;
    float getTransparency() const;
};
