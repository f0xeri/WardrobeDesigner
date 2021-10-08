//
// Created by Yaroslav on 21.09.2021.
//

#include "Object/WardrobeElements/WardrobeVerticalElement.hpp"
#include "Logger.hpp"

void WardrobeVerticalElement::update(State *state, size_t currentId) {
    size = cm->get_scale(c);
    if(!picked)
        position = cm->get_pos(c);
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
    cm->try_move_separator(c,position.x - prev_pos);
    prev_pos = position.x;
    Cube::end_move();
}
