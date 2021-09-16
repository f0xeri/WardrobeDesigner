//
// Created by Yaroslav on 25.02.2021.
//

#ifndef LAB4B_CUBE_HPP
#define LAB4B_CUBE_HPP

#include "IObject.hpp"

class Cube : public IObject {
public:
    Cube(glm::vec3 position, glm::vec3 size) : IObject(position), size(size),picked(false) {};
    glm::vec3 size;

    bool picked;

    struct plane_t
    {
        glm::vec3 origin;
        glm::vec3 normal;
    } movement_plane;
    glm::vec3 origin_offset;

    void start_move(State *state);
    void end_move();

    void generateVAO() override;
    void applyTranslations() override;
    void update(State *state, size_t currentId) override;
    void draw() override;
};



#endif //LAB4B_CUBE_HPP
