//
// Created by Yaroslav on 21.09.2021.
//

#include "Object/WardrobeElements/WardrobeHorizontalShelf.hpp"

void WardrobeHorizontalShelf::update(State *state, size_t currentId) {
    size = cm->getScale(c);
    if(!picked)
        position = cm->getPos(c);
    Cube::update(state, currentId);
}

void WardrobeHorizontalShelf::draw() {
    Cube::draw();
}

void WardrobeHorizontalShelf::start_move(State* s)
{
    prev_pos = position.y;
    Cube::start_move(s);
}

void WardrobeHorizontalShelf::end_move()
{
    cm->tryMoveSeparator(c, from_mm(position.y - prev_pos));
    prev_pos = position.y;
    Cube::end_move();
}
