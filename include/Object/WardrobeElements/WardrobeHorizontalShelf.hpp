//
// Created by Yaroslav on 21.09.2021.
//

#ifndef FOXENGINE_WARDROBEHORIZONTALSHELF_HPP
#define FOXENGINE_WARDROBEHORIZONTALSHELF_HPP


#include <Object/Cube.hpp>
#include "Tools.hpp"
#include "Cluster.h"

class WardrobeHorizontalShelf : public Cube
{
private:
    float prev_pos;
    ClusterManager* cm;
public:
    Cluster* c;
    WardrobeHorizontalShelf(Cluster* c, ClusterManager* cm) : c(c), Cube(glm::vec3(), glm::vec3())
    {
        this->cm = cm;
        size = cm->getScale(c);
        position = cm->getPos(c);
        moveAxis = {0, 1, 0};
        constraint_min = {-1000,-1000,-1000};
        constraint_max = {1000,1000,1000};
    };

    void update(State *state, size_t currentId) override;
    void draw() override;
    void start_move(State* s) override;
    void end_move() override;
    
};


#endif //FOXENGINE_WARDROBEHORIZONTALSHELF_HPP
