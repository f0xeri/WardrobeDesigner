//
// Created by Yaroslav on 21.09.2021.
//

#ifndef FOXENGINE_WARDROBEGENERATOR_HPP
#define FOXENGINE_WARDROBEGENERATOR_HPP


#include <glm/glm.hpp>
#include <Object/Cube.hpp>
#include <Object/WardrobeElements/WardrobeEdge.hpp>

class WardrobeGenerator {

public:
    WardrobeEdge *bottomSide;
    WardrobeEdge *backSide;
    WardrobeEdge *topSide;
    WardrobeEdge *leftSide;
    WardrobeEdge *rightSide;

    long double width, height, depth, boardThickness, baseHeight;

    WardrobeGenerator(glm::vec3 pos, long double width, long double height, long double depth, long double boardThickness, long double baseHeight, Texture *texture);
};


#endif //FOXENGINE_WARDROBEGENERATOR_HPP
