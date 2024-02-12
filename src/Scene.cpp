#include "Scene.hpp"

Scene::Scene()
{
    
}

Scene::~Scene()
{

}

void Scene::addObject(Rendered* o)
{
    this->objects.push_back(o);
}

std::vector<Rendered*> Scene::getObjects() const
{
    return this->objects;
}
