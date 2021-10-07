//
// Created by Yaroslav on 21.09.2021.
//

#include "Object/WardrobeElements/WardrobeVerticalElement.hpp"

void WardrobeVerticalElement::update(State *state, size_t currentId) {
    size.x = c->separator_width;
    size.y = c->claster_scale.y;
    constraint_min.x = c->BMin + offset;
    constraint_max.x = c->BMax - c->separator_width + offset;

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
    c->move_separator(position.x - prev_pos);
    prev_pos = position.x;
    Cube::end_move();
}
