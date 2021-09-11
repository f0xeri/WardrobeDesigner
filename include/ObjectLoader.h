//
// Created by Yaroslav on 30.10.2020.
//

#ifndef LAB4B_OBJECTLOADER_H
#define LAB4B_OBJECTLOADER_H
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "../Model.h"

class ObjectLoader
{
public:
    ObjectLoader(){};
    std::tuple<std::vector<glm::fvec3>, std::vector<glm::fvec2>, std::vector<glm::fvec3>> load(const std::string &filename, glm::vec3 pos = {0, 0, 0});
};

#endif //LAB4B_OBJECTLOADER_H
