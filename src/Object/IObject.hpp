//
// Created by Yaroslav on 25.02.2021.
//

#ifndef LAB4B_BASEOBJECT_HPP
#define LAB4B_BASEOBJECT_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>

struct VertexVectorStruct
{
public:
    std::vector<glm::fvec3> vertexPositions;
    std::vector<glm::fvec2> vertexTexCoords;
    std::vector<glm::fvec3> vertexNormals;
};

class IObject
{
public:
    IObject(glm::vec3 position) : position(position) {};
    glm::vec3 position;
    VertexVectorStruct objectData;
    GLuint VAO{};

    virtual void generateVAO() {};
    virtual void update(float dt) {};
    virtual void draw() {};

    bool physicsEnabled = false;
    bool collisionEnabled = false;

    std::vector<GLuint> mBuffers;
    unsigned mIndicesBuffer = 0;
    size_t mIndicesCount = 0;
};


#endif //LAB4B_BASEOBJECT_HPP
