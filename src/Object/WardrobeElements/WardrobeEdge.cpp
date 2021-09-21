//
// Created by Yaroslav on 21.09.2021.
//

#include "Object/WardrobeElements/WardrobeEdge.hpp"

void WardrobeEdge::update(State *state, size_t currentId) {
    applyTranslations();
}

void WardrobeEdge::draw() {
    if (enabled)
        Cube::draw();
}
