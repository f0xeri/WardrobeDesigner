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

struct State;

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
    IObject(glm::vec3 position) : position(position), startPosition(position), model(glm::mat4(1.0f)), picked(false) {};
    glm::vec3 position;
    glm::vec3 startPosition;
    glm::mat4 model;
    bool picked;
    struct plane_t
    {
        glm::vec3 origin;
        glm::vec3 normal;
    } movement_plane;
    Texture *texture;
    int texScaleX = 1;
    int texScaleY = 1;
    VertexVectorStruct objectData;
    GLuint VAO{};

    virtual void generateVAO() {};
    virtual void update(State *state, size_t currentId) {};
    virtual void applyTranslations() {};
    virtual void draw() {};

    virtual void start_move(State *state) {};
    virtual void end_move() {};

    bool physicsEnabled = false;
    bool collisionEnabled = false;

    std::vector<GLuint> mBuffers;
    unsigned mIndicesBuffer = 0;
    size_t mIndicesCount = 0;
};

#endif //LAB4B_BASEOBJECT_HPP
