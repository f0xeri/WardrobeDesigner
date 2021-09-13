//
// Created by Yaroslav on 25.02.2021.
//

#ifndef LAB4B_BASEOBJECT_HPP
#define LAB4B_BASEOBJECT_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <tuple>
#include "Texture.hpp"


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
    IObject(glm::vec3 position) : position(position), startPosition(position), model(glm::mat4(1.0f)) {};
    glm::vec3 position;
    glm::vec3 startPosition;
    glm::mat4 model;
    Texture *texture;
    int texScaleX = 1;
    int texScaleY = 1;
    VertexVectorStruct objectData;
    GLuint VAO{};

    virtual void generateVAO() {};
    virtual void update(float dt) {};
    virtual void applyTranslations() {};
    virtual void draw() {};

    bool physicsEnabled = false;
    bool collisionEnabled = false;

    std::vector<GLuint> mBuffers;
    unsigned mIndicesBuffer = 0;
    size_t mIndicesCount = 0;
};

#endif //LAB4B_BASEOBJECT_HPP
