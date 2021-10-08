//
// Created by Yaroslav on 21.09.2021.
//

#ifndef FOXENGINE_WARDROBEEDGE_HPP
#define FOXENGINE_WARDROBEEDGE_HPP


#include <Object/Cube.hpp>

class WardrobeEdge : public Cube
{
public:
    WardrobeEdge(glm::vec3 position, glm::vec3 size) : Cube(position, size) {};
    void update(State *state, size_t currentId) override;
    void draw() override;
};


#endif //FOXENGINE_WARDROBEEDGE_HPP
