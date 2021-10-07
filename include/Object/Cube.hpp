//
// Created by Yaroslav on 25.02.2021.
//

#ifndef LAB4B_CUBE_HPP
#define LAB4B_CUBE_HPP

#include "IObject.hpp"

class Cube : public IObject {
public:
    Cube(glm::vec3 position, glm::vec3 size) : IObject(position), size(size), startSize(size) {};
    glm::vec3 size;
    glm::vec3 startSize;

    glm::vec3 Bmax, Bmin;
    struct plane_t
    {
        glm::vec3 origin;
        glm::vec3 normal;
    } movement_plane;
    glm::vec3 origin_offset;

    glm::vec3 constraint_min;
    glm::vec3 constraint_max;
    

    void start_move(State *state);
    void end_move();

    void generateVAO() override;
    void applyTranslations() override;
    void update(State *state, size_t currentId) override;
    void draw() override;

    bool intersect(const glm::vec3 &rayPos, const glm::vec3 &rayDir, float &t) const;
};



#endif //LAB4B_CUBE_HPP
