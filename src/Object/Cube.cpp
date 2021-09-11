//
// Created by Yaroslav on 25.02.2021.
//

#include "Object/Cube.hpp"
#include "Logger.hpp"

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

    Bmin = {position.x, position.y, position.z};
    Bmax = {size.x + position.x, size.y + position.y, size.z + position.z};

    std::vector<glm::vec2> cubeTexCoords;
    cubeTexCoords.reserve(6);
    for (int i = 0; i < 6; ++i)
    {
        cubeTexCoords.insert(cubeTexCoords.end(), {
                {0, 2048},
                {2048, 2048},
                {2048, 0},
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

void Cube::draw() {
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
