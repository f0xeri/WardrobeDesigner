//
// Created by Yaroslav on 21.09.2021.
//

#include "WardrobeGenerator.hpp"

WardrobeGenerator::WardrobeGenerator(glm::vec3 pos, long double width, long double height, long double depth, long double boardThickness, long double baseHeight, Texture *texture)
                                                    : origin(pos.x + boardThickness,pos.y + baseHeight,pos.z - depth), width(width), height(height), depth(depth), boardThickness(boardThickness), baseHeight(baseHeight)
{
    bottomSide = new WardrobeEdge({pos.x, pos.y, -depth}, {width, baseHeight, depth});
    backSide = new WardrobeEdge(pos, {width, height, boardThickness});
    topSide = new WardrobeEdge({boardThickness, height - boardThickness, -depth}, {width - boardThickness * 2, boardThickness, depth});
    leftSide = new WardrobeEdge({pos.x, baseHeight, -depth}, {boardThickness, height - baseHeight, depth});
    rightSide = new WardrobeEdge({width - boardThickness, baseHeight, -depth}, {boardThickness, height - baseHeight, depth});

    bottomSide->texture = texture;
    backSide->texture = texture;
    topSide->texture = texture;
    leftSide->texture = texture;
    rightSide->texture = texture;

    bottomSide->generateVAO();
    backSide->generateVAO();
    topSide->generateVAO();
    leftSide->generateVAO();
    rightSide->generateVAO();
}
