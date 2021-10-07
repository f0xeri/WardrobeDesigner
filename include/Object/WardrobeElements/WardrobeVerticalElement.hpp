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
    
    float offset;
    float prev_pos;
public:
    Cluster* c;
    WardrobeVerticalElement(glm::vec3 position,Cluster* c, float depth,float offset) : 
        Cube(position, glm::vec3(c->separator_width,c->claster_scale.y,depth)), offset(offset),
        c(c) 
    {
        constraint_min = {c->BMin + offset,-1000,-1000};
        constraint_max = {c->BMax + offset,1000,1000};

        moveAxis = {1, 0, 0};
    };
    void update(State *state, size_t currentId) override;
    void draw() override;
    void start_move(State* s) override;
    void end_move() override;
    
};


#endif //FOXENGINE_WARDROBEVERTICALELEMENT_HPP
