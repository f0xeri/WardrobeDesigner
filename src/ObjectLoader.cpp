//
// Created by Yaroslav on 30.10.2020.
//

#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <tuple>
#include <cstring>
#include "ObjectLoader.h"
#include "Logger.hpp"

std::tuple<std::vector<glm::fvec3>, std::vector<glm::fvec2>, std::vector<glm::fvec3>> ObjectLoader::load(
        const std::string &filename, glm::vec3 pos)
{
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::vector<glm::vec3> out_vertices;
    std::vector<glm::vec2> out_uvs;
    std::vector<glm::vec3> out_normals;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        LOG("[ERROR] Failed to open " + filename + ".");
        return VertexVector {};
    }

    std::stringstream ss;
    std::string line;
    std::string prefix;
    while (std::getline(file, line))
    {
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "v")
        {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            vertex.x += pos.x;
            vertex.y += pos.y;
            vertex.z += pos.z;
            temp_vertices.push_back(vertex);
        }
        else if (prefix == "vt")
        {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            uv.y = -uv.y;
            temp_uvs.push_back(uv);
        }
        else if (prefix == "vn")
        {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        else if (prefix == "f")
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            char ch;
            ss >> vertexIndex[0] >> ch >> uvIndex[0] >> ch >> normalIndex[0];
            ss >> vertexIndex[1] >> ch >> uvIndex[1] >> ch >> normalIndex[1];
            ss >> vertexIndex[2] >> ch >> uvIndex[2] >> ch >> normalIndex[2];

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        else continue;
    }

    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        glm::vec2 uv = temp_uvs[uvIndex - 1];
        glm::vec3 normal = temp_normals[normalIndex - 1];

        out_vertices.push_back(vertex);
        out_uvs.push_back(uv);
        out_normals.push_back(normal);
    }
    LOG("[INFO] Object " + filename + " loaded.")
    return std::make_tuple(out_vertices, out_uvs, out_normals);
}
