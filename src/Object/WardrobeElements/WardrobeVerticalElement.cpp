//
// Created by Yaroslav on 21.09.2021.
//

#include "Object/WardrobeElements/WardrobeVerticalElement.hpp"
#include "Logger.hpp"

void WardrobeVerticalElement::update(State *state, size_t currentId) {
    size = cm->getScale(c);
    if(!picked)
        position = cm->getPos(c);
    Cube::update(state, currentId);
}

void WardrobeVerticalElement::draw() {
    Cube::draw();
}

void WardrobeVerticalElement::start_move(State* s)
{
    prev_pos = position.x;
    Cube::start_move(s);
}

void WardrobeVerticalElement::end_move()
{
    cm->tryMoveSeparator(c, from_mm(position.x - prev_pos));
    prev_pos = position.x;
    Cube::end_move();
}
