//
// Created by Yaroslav on 22.09.2021.
//

#ifndef FOXENGINE_TOOLS_HPP
#define FOXENGINE_TOOLS_HPP

#include <glm/vec3.hpp>

long double operator "" _mm(long double mm);
long double operator "" _mm(unsigned long long mm);
float from_mm(float mm);
long double from_mm(int mm);
float to_mm(float mm);
long double to_mm(int mm);
int inline GetIntersection(float fDst1, float fDst2, glm::vec3 P1, glm::vec3 P2, glm::vec3 &Hit);
int inline InBox( glm::vec3 Hit, glm::vec3 B1, glm::vec3 B2, const int Axis);
bool checkRayCubeIntercection(glm::vec3 B1, glm::vec3 B2, glm::vec3 L1, glm::vec3 L2, glm::vec3 &Hit);
bool within(glm::vec3 min, glm::vec3 max, glm::vec3 v);

#define EPS 0.0000001
#define FLOAT_EQUAL(a, b) (fabs(a - b) < EPS)


#endif //FOXENGINE_TOOLS_HPP
