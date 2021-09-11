//
// Created by Yaroslav on 25.02.2021.
//

#ifndef LAB4B_SPHERE_HPP
#define LAB4B_SPHERE_HPP

#include "IObject.hpp"
#include <glm/glm.hpp>

class Sphere : public IObject {
public:
    float radius;

    float mass;
    glm::vec3 velocity{};
    glm::vec3 forceVector{};

    std::vector<float> vertices;
    std::vector<float> texCoords;
    std::vector<float> normals;
    std::vector<int> indices;
    std::vector<int> lineIndices;

    void generateVAO() override;
    void draw() override;

    Sphere(glm::vec3 position, float radius) : IObject(position), radius(radius) {}

    void update(float dt, bool col, float y);
};


#endif //LAB4B_SPHERE_HPP
