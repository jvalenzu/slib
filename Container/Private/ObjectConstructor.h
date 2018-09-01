// -*- mode: c++; tab-width: 4; c-basic-offset: 4; -*-
#pragma once

template <typename T, bool kDefaultConstruct=true>
struct ObjectConstructor
{
    ObjectConstructor() = default;
    ObjectConstructor(T* area)
    {
        new (area) T();
    }
};

template <typename T>
struct ObjectConstructor<T, false>
{
    ObjectConstructor(T* area)
    {
    }
};
