//
// Created by Yaroslav on 25.02.2021.
//

#include "Object/Cube.hpp"
#include "Logger.hpp"
#include "State.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/glm.hpp>
#include <Tools.hpp>

void Cube::generateVAO()
{
    std::vector<glm::vec3> cube = {
        // top
        {0, 1, 1},
        {1, 1, 1},
        {1, 1, 0},
        {0, 1, 0},

        // back
        {1, 0, 0},
        {0, 0, 0},
        {0, 1, 0},
        {1, 1, 0},

        {0, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 1, 1},

        {0, 0, 0},
        {1, 0, 0},
        {1, 0, 1},
        {0, 0, 1},

        {1, 0, 1},
        {1, 0, 0},
        {1, 1, 0},
        {1, 1, 1},

        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 1},
        {0, 1, 0}};

    //Bmin = {position.x, position.y, position.z};
    //Bmax = {1 + position.x, 1 + position.y, 1 + position.z};

    std::vector<glm::vec2> cubeTexCoords;
    cubeTexCoords.reserve(6);
    for (int i = 0; i < 6; ++i)
    {
        cubeTexCoords.insert(cubeTexCoords.end(), {{0, texScaleY},
                                                   {texScaleX, texScaleY},
                                                   {texScaleX, 0},
                                                   {0, 0}});
    }

    float dImin = 0.0f; // diffuseIntesitivityMin
    float dImax = 1.0f; // diffuseIntesitivityMax
    std::vector<glm::vec3> normals = {
        {dImin, dImax, dImin},
        {dImin, dImax, dImin},
        {dImin, dImax, dImin},
        {dImin, dImax, dImin},

        {dImin, dImin, -dImax},
        {dImin, dImin, -dImax},
        {dImin, dImin, -dImax},
        {dImin, dImin, -dImax},

        {dImin, dImin, dImax},
        {dImin, dImin, dImax},
        {dImin, dImin, dImax},
        {dImin, dImin, dImax},

        {dImin, -dImax, dImin},
        {dImin, -dImax, dImin},
        {dImin, -dImax, dImin},
        {dImin, -dImax, dImin},

        {dImax, dImin, dImin},
        {dImax, dImin, dImin},
        {dImax, dImin, dImin},
        {dImax, dImin, dImin},

        {-dImax, dImin, dImin},
        {-dImax, dImin, dImin},
        {-dImax, dImin, dImin},
        {-dImax, dImin, dImin}};

    std::vector<int> cubeIndices;
    cubeIndices.reserve(6);
    for (int i = 0; i < 6; ++i)
    {
        cubeIndices.insert(cubeIndices.end(),
                           {0 + i * 4, 1 + i * 4, 2 + i * 4,
                            2 + i * 4, 3 + i * 4, 0 + i * 4});
    }

    GLuint VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cube.size() * sizeof(glm::vec3), cube.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(mBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    mBuffers.push_back(VBO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeTexCoords.size() * sizeof(glm::vec2), cubeTexCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(mBuffers.size(), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    mBuffers.push_back(VBO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(mBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    mBuffers.push_back(VBO);

    glGenBuffers(1, &mIndicesBuffer);
    mIndicesCount = cubeIndices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned), cubeIndices.data(), GL_STATIC_DRAW);
}

void Cube::applyTranslations()
{
    model = mat4(1.0f);
    Bmin = {position.x, position.y, position.z};
    Bmax = {size.x + position.x, size.y + position.y, size.z + position.z};
    model = glm::translate(model, position);
    model = glm::scale(model, size);
}

vec3 delta;

bool Cube::intersect(const vec3 &rayPos, const vec3 &rayDir, float &t) const
{
    float t1 = INFINITY, t0 = -t1;

    for (int i = 0; i < 3; i++)
    {
        if (rayDir[i] != 0)
        {
            float lo = (Bmin[i] - rayPos[i]) / rayDir[i];
            float hi = (Bmax[i] - rayPos[i]) / rayDir[i];
            t0 = glm::max(t0, glm::min(lo, hi));
            t1 = glm::min(t1, glm::max(lo, hi));
        }
        else
        {
            if (rayPos[i] < Bmin[i] || rayPos[i] > Bmax[i])
                return false;
        }
    }
    t = t0;
    return (t0 <= t1) && (t1 > 0);
}

void Cube::start_move(State *state)
{
    glm::vec3 ray_start = state->arcBallCamera->pos;
    glm::vec3 ray_dir = state->arcBallCamera->raycastFromViewportCoords(state->dx, state->dy);
    glm::vec3 ray_end = ray_start + ray_dir * 1000.0f;
    glm::vec3 hit;
    Bmin = {position.x, position.y, position.z};
    Bmax = {size.x + position.x, size.y + position.y, size.z + position.z};
    bool result = checkRayCubeIntercection(Bmin, Bmax, ray_start, ray_end, hit);
    movement_plane.origin = vec3(0, 0, hit.z);
    movement_plane.normal = vec3(0, 0, 1);
    float intersect_distance = 0;
    origin_offset = hit - position;
    picked = true;
}

void Cube::end_move()
{
    picked = false;
}

void Cube::update(State *state, size_t currentId)
{
    applyTranslations();
    if (picked)
    {
        glm::vec3 ray_start = state->arcBallCamera->pos;
        glm::vec3 ray_dir = glm::normalize(state->arcBallCamera->raycastFromViewportCoords(state->x, state->y));
        float intersect_distance = 0;
        glm::intersectRayPlane(ray_start, ray_dir, movement_plane.origin, movement_plane.normal, intersect_distance);
        glm::vec3 intersect_pos = ray_start + ray_dir * intersect_distance;

        intersect_pos = intersect_pos - origin_offset;

        if (!within(constraint_min, constraint_max, intersect_pos))
            return;

        if (moveAxis.x != 0)
        {
            if (intersect_pos.x > constraint_max.x)
            {
                position.x = constraint_max.x;
            }
            else if (intersect_pos.x < constraint_min.x)
            {
                position.x = constraint_min.x;
            }
            else
                position.x = intersect_pos.x;
        }

        if (moveAxis.y != 0)
        {
            if (intersect_pos.y > constraint_max.y)
            {
                position.y = constraint_max.y;
            }
            else if (intersect_pos.y < constraint_min.y)
            {
                position.y = constraint_min.y;
            }
            else
                position.y = intersect_pos.y;
        }

        if (moveAxis.z != 0)
        {
            if (intersect_pos.z > constraint_max.z)
            {
                position.z = constraint_max.z;
            }
            else if (intersect_pos.z < constraint_min.z)
            {
                position.z = constraint_min.z;
            }
            else
                position.z = intersect_pos.z;
        }
    }
}

void Cube::draw()
{
    assert(mIndicesBuffer != 0);
    glBindVertexArray(VAO);
    for (size_t i = 0; i < mBuffers.size(); ++i)
    {
        glEnableVertexAttribArray(i);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
    glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, nullptr);
    for (size_t i = 0; i < mBuffers.size(); ++i)
    {
        glDisableVertexAttribArray(i);
    }
}