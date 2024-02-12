#include "Object.hpp"

Object::Object(glm::vec3 p, glm::vec3 r)
    : position(p), rotation(r)
{

}

Object::~Object()
{

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
