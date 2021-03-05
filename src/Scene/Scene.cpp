//
// Created by Yaroslav on 01.03.2021.
//

#include "Scene.hpp"

void Scene::addObject(IObject *object)
{
    objects.emplace_back(object);
}
