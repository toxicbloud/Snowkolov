#pragma once
#include "Object.hpp"

class Camera: public Object
{
public: // TODO : change
    float fov;
public:
    Camera(glm::vec3 p, glm::vec3 r, float f);
    ~Camera();
    glm::mat3 getViewMatrix() const;
};
