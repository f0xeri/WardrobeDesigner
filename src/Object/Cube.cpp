//
// Created by Yaroslav on 25.02.2021.
//

#include "Object/Cube.hpp"
#include "Logger.hpp"
#include "State.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>

void Cube::generateVAO() {
    std::vector<glm::vec3> cube = {
            // top
            {0 + position.x, size.y + position.y, size.z + position.z},
            {size.x + position.x, size.y + position.y, size.z + position.z},
            {size.x + position.x, size.y + position.y, 0 + position.z},
            {0 + position.x, size.y + position.y, 0 + position.z},

            // back
            {size.x + position.x, 0 + position.y, 0 + position.z},
            {0 + position.x, 0 + position.y, 0 + position.z},
            {0 + position.x, size.y + position.y, 0 + position.z},
            {size.x + position.x, size.y + position.y, 0 + position.z},

            {0 + position.x, 0 + position.y, size.z + position.z},
            {size.x + position.x, 0 + position.y, size.z + position.z},
            {size.x + position.x, size.y + position.y, size.z + position.z},
            {0 + position.x, size.y + position.y, size.z + position.z},

            {0 + position.x, 0 + position.y, 0 + position.z},
            {size.x + position.x, 0 + position.y, 0 + position.z},
            {size.x + position.x, 0 + position.y, size.z + position.z},
            {0 + position.x, 0 + position.y, size.z + position.z},

            {size.x + position.x, 0 + position.y, size.z + position.z},
            {size.x + position.x, 0 + position.y, 0 + position.z},
            {size.x + position.x, size.y + position.y, 0 + position.z},
            {size.x + position.x, size.y + position.y, size.z + position.z},

            {0 + position.x, 0 + position.y, 0 + position.z},
            {0 + position.x, 0 + position.y, size.z + position.z},
            {0 + position.x, size.y + position.y, size.z + position.z},
            {0 + position.x, size.y + position.y, 0 + position.z}
    };

    //Bmin = {position.x, position.y, position.z};
    //Bmax = {size.x + position.x, size.y + position.y, size.z + position.z};

    std::vector<glm::vec2> cubeTexCoords;
    cubeTexCoords.reserve(6);
    for (int i = 0; i < 6; ++i)
    {
        cubeTexCoords.insert(cubeTexCoords.end(), {
                {0, texScaleY},
                {texScaleX, texScaleY},
                {texScaleX, 0},
                {0, 0}
        });
    }

    float dImin = 0.0f;  // diffuseIntesitivityMin
    float dImax = 1.0f;  // diffuseIntesitivityMax
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
            {-dImax, dImin, dImin}
    };

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

void Cube::applyTranslations() {
    model = mat4(1.0f);
    model = glm::scale(model, size / startSize);
    model = glm::translate(model, position - startPosition);
}

int inline GetIntersection(float fDst1, float fDst2, vec3 P1, vec3 P2, vec3 &Hit) {
    if ((fDst1 * fDst2) >= 0.0f) return 0;
    if (fDst1 == fDst2) return 0;
    Hit = P1 + (P2 - P1) * (-fDst1/(fDst2-fDst1) );
    return 1;
}

int inline InBox( vec3 Hit, vec3 B1, vec3 B2, const int Axis) {
    if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return 1;
    if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return 1;
    if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return 1;
    return 0;
}

// returns true if line (L1, L2) intersects with the box (B1, B2)
// returns intersection point in Hit
bool checkRayCubeIntercection(vec3 B1, vec3 B2, vec3 L1, vec3 L2, vec3 &Hit)
{
    if (L2.x < B1.x && L1.x < B1.x) return false;
    if (L2.x > B2.x && L1.x > B2.x) return false;
    if (L2.y < B1.y && L1.y < B1.y) return false;
    if (L2.y > B2.y && L1.y > B2.y) return false;
    if (L2.z < B1.z && L1.z < B1.z) return false;
    if (L2.z > B2.z && L1.z > B2.z) return false;
    if (L1.x > B1.x && L1.x < B2.x && L1.y > B1.y && L1.y < B2.y && L1.z > B1.z && L1.z < B2.z)
    {
        Hit = L1;
        return true;
    }
    if ( (GetIntersection( L1.x-B1.x, L2.x-B1.x, L1, L2, Hit) && InBox( Hit, B1, B2, 1 ))
         || (GetIntersection( L1.y-B1.y, L2.y-B1.y, L1, L2, Hit) && InBox( Hit, B1, B2, 2 ))
         || (GetIntersection( L1.z-B1.z, L2.z-B1.z, L1, L2, Hit) && InBox( Hit, B1, B2, 3 ))
         || (GetIntersection( L1.x-B2.x, L2.x-B2.x, L1, L2, Hit) && InBox( Hit, B1, B2, 1 ))
         || (GetIntersection( L1.y-B2.y, L2.y-B2.y, L1, L2, Hit) && InBox( Hit, B1, B2, 2 ))
         || (GetIntersection( L1.z-B2.z, L2.z-B2.z, L1, L2, Hit) && InBox( Hit, B1, B2, 3 )))
        return true;

    return false;
}

vec3 delta;

void Cube::start_move(State *state)
{
    glm::vec3 ray_start = state->camera->pos;
    glm::vec3 ray_dir = state->camera->raycastFromViewportCoords(state->dx, state->dy);
    glm::vec3 ray_end = ray_start + ray_dir * 1000.0f;
    glm::vec3 hit;
    glm::vec3 Bmin, Bmax;
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
    if (picked)
    {
        glm::vec3 ray_start = state->camera->pos;
        glm::vec3 ray_dir = glm::normalize(state->camera->raycastFromViewportCoords(state->x, state->y));
        float intersect_distance = 0;
        glm::intersectRayPlane(ray_start, ray_dir, movement_plane.origin, movement_plane.normal, intersect_distance);
        glm::vec3 intersect_pos = ray_start + ray_dir * intersect_distance;
        position.x = (intersect_pos - origin_offset).x;
    }
    applyTranslations();
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