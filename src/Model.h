//
// Created by Yaroslav on 30.10.2020.
//

#include <glm/glm.hpp>

#ifndef LAB4B_VERTEX_H
#define LAB4B_VERTEX_H

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
    glm::vec3 normal;
};

using VertexVector = std::tuple<std::vector<glm::fvec3>, std::vector<glm::fvec2>, std::vector<glm::fvec3>>;

#endif //LAB4B_VERTEX_H


