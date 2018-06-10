// -*- mode: c++; tab-width: 4; c-basic-offset: 4; -*-
#pragma once

#include <math.h>
#include <stdint.h>

namespace Utils
{
    template <typename T>
    void swap(T& a, T& b)
    {
        T temp = a;
        a = b;
        b = temp;
    }
}

#define OFFSETOF(s,m)        (((size_t)(&((s*)0x10)->m)) - 0x10)
#define BUFFER_OFFSETOF(s,m) ((char *)nullptr + (offsetof(s,m)))
#define MIN(a,b)             ((a)<(b)?(a):(b))
#define ELEMENTSOF(a)        (sizeof(a)/(sizeof(a)[0]))
#define ELEMENTSOF_2R(a)     (sizeof(a)/sizeof((a)[0]))
#define ELEMENTSOF_2C(a)     (sizeof((a)[0])/sizeof((a)[0][0]))

inline bool FloatEqual(float a, float b, float epsilon)
{
    return fabsf(a-b) < epsilon;
}

// http://www.cse.yorku.ca/~oz/hash.html
inline uint32_t Djb(const char* str)
{
    uint32_t hash = 5381;
    int c;
    
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash;
}

template <typename T, typename U>
inline T Min(T v0, U v1)
{
    if (v0 < v1)
        return v0;
    return v1;
}

template <typename T, typename U>
inline T Max(T v0, U v1)
{
    if (v0 < v1)
        return v1;
    return v0;
}

inline float Clamp(float value, float v0, float v1)
{
    const float v0p = Min(v0, v1);
    const float v1p = Max(v0, v1);
    
    return Min(Max(value, v0p), v1p);
}
