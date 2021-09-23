//
// Created by Yaroslav on 22.09.2021.
//

#include "Tools.hpp"

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