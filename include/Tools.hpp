//
// Created by Yaroslav on 22.09.2021.
//

#ifndef FOXENGINE_TOOLS_HPP
#define FOXENGINE_TOOLS_HPP
#include "Tools.hpp"

long double operator "" _mm(long double mm);
long double operator "" _mm(unsigned long long mm);
float from_mm(float mm);
long double from_mm(int mm);
float to_mm(float mm);
long double to_mm(int mm);
int inline GetIntersection(float fDst1, float fDst2, vec3 P1, vec3 P2, vec3 &Hit);
int inline InBox( vec3 Hit, vec3 B1, vec3 B2, const int Axis);
bool checkRayCubeIntercection(vec3 B1, vec3 B2, vec3 L1, vec3 L2, vec3 &Hit);

#endif //FOXENGINE_TOOLS_HPP
