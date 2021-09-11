//
// Created by Yaroslav on 25.02.2021.
//

#include <glm/ext/scalar_constants.hpp>
#include "Object/Sphere.hpp"
#include "Logger.hpp"

void Sphere::generateVAO() {
    forceVector = {0, -39.8, 0};
    mass = 1;

    float PI = 3.14159265358979323846264338327950288;

    float sectorCount = 36.0f;
    float stackCount = 18.0f;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x + position.x);
            vertices.push_back(y + position.y);
            vertices.push_back(z + position.z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            texCoords.push_back(s);
            texCoords.push_back(t);
        }
    }

    int k1, k2;
    for(int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (stackCount-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if(i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    GLuint VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(mBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    mBuffers.push_back(VBO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(mBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    mBuffers.push_back(VBO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(mBuffers.size(), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    mBuffers.push_back(VBO);


    glGenBuffers(1, &mIndicesBuffer);
    mIndicesCount = indices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);
}

void Sphere::draw() {

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

void Sphere::update(float dt, bool col, float y)
{
    if (!physicsEnabled) return;
    // LOG(position.y)
    velocity += forceVector * dt;
    position = position + velocity * dt - (forceVector * (dt * dt)) / glm::vec3(2.0f, 2.0f, 2.0f);
    if (col && abs(velocity.y) < 1)
    {
        position.y = y + radius * 2;
        velocity.y = 0;
    }
    else if(col && abs(velocity.y) > 1)
    {
        position.y = y + radius * 2;
        velocity.x = velocity.x - (velocity.x / 5);
        velocity.y = -velocity.y + (velocity.y / 5);
        velocity.z = velocity.z - (velocity.z / 5);
    }
    if (col && abs(velocity.x) < 1)
    {
        velocity.x = 0;
    }
    if (col && abs(velocity.z) < 1)
    {
        velocity.z = 0;
    }
}