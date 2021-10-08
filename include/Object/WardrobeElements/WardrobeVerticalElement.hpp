//
// Created by Yaroslav on 21.09.2021.
//

#ifndef FOXENGINE_WARDROBEVERTICALELEMENT_HPP
#define FOXENGINE_WARDROBEVERTICALELEMENT_HPP


#include <Object/Cube.hpp>
#include "Tools.hpp"
#include "Cluster.h"

class WardrobeVerticalElement : public Cube
{
private:
    
    float prev_pos;
    Claster_manager* cm;
public:
    Claster* c;
    WardrobeVerticalElement(Claster* c, Claster_manager* cm) : c(c), Cube(glm::vec3(),glm::vec3())
    {
        this->cm = cm;
        size = cm->get_scale(c);
        position = cm->get_pos(c);
        moveAxis = {1, 0, 0};
        constraint_min = {-1000,-1000,-1000};
        constraint_max = {1000,1000,1000};
    };

    void update(State *state, size_t currentId) override;
    void draw() override;
    void start_move(State* s) override;
    void end_move() override;
    
};


#endif //FOXENGINE_WARDROBEVERTICALELEMENT_HPP
