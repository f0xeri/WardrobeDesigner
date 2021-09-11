//
// Created by Yaroslav on 01.03.2021.
//

#ifndef LAB4B_SCENE_HPP
#define LAB4B_SCENE_HPP

#include <vector>
#include "../Object/IObject.hpp"


class Scene
{
public:
    std::vector<IObject*> objects;
    void addObject(IObject *object);
};


#endif //LAB4B_SCENE_HPP
