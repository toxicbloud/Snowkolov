#pragma once
#include "Object.hpp"

class Camera: public Object
{
public:
    Camera(glm::vec3 p, glm::vec3 r);
    ~Camera();
    glm::mat3 getViewMatrix() const;
};
