#include "Object.hpp"

Object::Object(glm::vec3 p, glm::vec3 r)
    : position(p), rotation(r)
{

}

Object::~Object()
{

}

glm::vec3 Object::getRotatedPoint(const glm::vec3 &p) const
{
    // change point coordinates to local coordinates
    glm::vec3 q = p - position;
    // create rotation matrix from euler angles
    glm::mat3 rot = glm::mat3(
        glm::vec3(cos(rotation.y) * cos(rotation.z), cos(rotation.y) * sin(rotation.z), -sin(rotation.y)),
        glm::vec3(sin(rotation.x) * sin(rotation.y) * cos(rotation.z) - cos(rotation.x) * sin(rotation.z), sin(rotation.x) * sin(rotation.y) * sin(rotation.z) + cos(rotation.x) * cos(rotation.z), sin(rotation.x) * cos(rotation.y)),
        glm::vec3(cos(rotation.x) * sin(rotation.y) * cos(rotation.z) + sin(rotation.x) * sin(rotation.z), cos(rotation.x) * sin(rotation.y) * sin(rotation.z) - sin(rotation.x) * cos(rotation.z), cos(rotation.x) * cos(rotation.y))
    );
    // rotate point
    q = rot * q;
    // change point coordinates to global coordinates
    return q + position;
}

glm::vec3 Object::getPosition() const
{
    return this->position;
}

glm::vec3 Object::getRotation() const
{
    return this->rotation;
}

void Object::setPosition(glm::vec3 p)
{
    this->position = p;
}

void Object::setRotation(glm::vec3 r)
{
    this->rotation = r;
}
