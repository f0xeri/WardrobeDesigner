//
// Created by Yaroslav on 25.02.2021.
//

#ifndef LAB4B_CUBE_HPP
#define LAB4B_CUBE_HPP

#include "IObject.hpp"

class Cube : public IObject {
public:
    Cube(glm::vec3 position, glm::vec3 size) : IObject(position), size(size) {};
    glm::vec3 size;

    glm::vec3 Bmin, Bmax;

    void generateVAO() override;
    void applyTranslations() override;
    void update(float dt) override;
    void draw() override;
};



#endif //LAB4B_CUBE_HPP
