#pragma once
#include <glm/glm.hpp>

#define PI 3.1415926f
#define deg2rad(x) x * PI / 180.f
#define rad2deg(x) x * 180.f / PI

class Object
{
protected:
    glm::vec3 position;
    glm::vec3 rotation;

public:
    Object(glm::vec3 p, glm::vec3 r);
    ~Object();

    glm::vec3 getRotatedPoint(const glm::vec3 &p) const;

    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;

    void setPosition(glm::vec3 p);
    void setRotation(glm::vec3 r);
};
