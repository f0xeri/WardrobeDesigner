//
// Created by Yaroslav on 21.09.2021.
//

#ifndef FOXENGINE_WARDROBEHORIZONTALSHELF_HPP
#define FOXENGINE_WARDROBEHORIZONTALSHELF_HPP

#include <Object/Cube.hpp>

class WardrobeHorizontalShelf : public Cube
{
public:
    WardrobeHorizontalShelf(glm::vec3 position, glm::vec3 size) : Cube(position, size) {};
    void update(State *state, size_t currentId) override;
    void draw() override;
};


#endif //FOXENGINE_WARDROBEHORIZONTALSHELF_HPP
