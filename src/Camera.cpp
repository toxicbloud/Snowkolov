#include "Camera.hpp"
#include <iostream>

Camera::Camera(glm::vec3 p, glm::vec3 r, float f)
    : Object(p, r), fov(f)
{
    
}

Camera::~Camera()
{
    
}

glm::mat3 Camera::getViewMatrix() const
{
    /**
     *  y
     * /\    z
     * ||  /|
     * || //
     * ||//
     * ========> x
    **/
    glm::vec3 cw = glm::normalize(rotation - position); // rotation is target position
    glm::vec3 cp = glm::vec3(0, 1, 0); // up vector
    glm::vec3 cu = glm::normalize(glm::cross(cw, cp));
    return glm::mat3(cu, glm::cross(cw, cu), cw);
}
