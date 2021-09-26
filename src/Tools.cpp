//
// Created by Yaroslav on 22.09.2021.
//

#include <glm/vec3.hpp>

long double operator "" _mm(long double mm)
{
    return mm / 320.0f;
}

long double operator "" _mm(unsigned long long mm)
{
    return mm / 320.0f;
}

float from_mm(float mm)
{
    return mm * 320.0f;
}

long double from_mm(int mm)
{
    return mm * 320.0f;
}

float to_mm(float mm)
{
    return mm / 320.0f;
}

long double to_mm(int mm)
{
    return mm / 320.0f;
}

using namespace glm;

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