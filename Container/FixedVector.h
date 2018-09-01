// -*- mode: c++; tab-width: 4; c-basic-offset: 4; -*-

#pragma once

#include "slib/Container/Private/ObjectConstructor.h"
#include <new>

template <typename T, int maxSize>
struct FixedVector
{
    enum
    {
        kMaxSize = maxSize
    };
    char m_Raw[maxSize * sizeof(T)];
    int m_Index = 0;
    
    FixedVector()
    {
    }
    
    inline T* Data()
    {
        return (T*)m_Raw;
    }
    
    inline const T* Data() const
    {
        return (const T*)m_Raw;
    }
    
    void SetCount(int count)
    {
        m_Index = count;
    }
    
    int Count() const
    {
        return m_Index;
    }
    
    T* Alloc()
    {
        if (m_Index == maxSize)
            return nullptr;
        T* ret = (T*)&m_Raw[m_Index++ * sizeof(T)];
        ObjectConstructor<T, std::is_default_constructible<T>::value> o(ret);
        return ret;
    }
    
    int IndexOf(const T* data)
    {
        int index = (int) (data - (T*)&m_Raw[0]);
        if (index <0 || index >= maxSize)
            return -1;
        return index;
    }
    
    void Erase(T* victim)
    {
        int index = victim - (T*)&m_Raw[0];
        if (index >= maxSize)
            return;
        // victim->~T();
        *victim = *(T*)m_Raw[--m_Index*sizeof(T)];
    }
    
    T operator[](size_t index) const
    {
        const T* array = Data();
        return array[index];
    }
    
    T& operator[](size_t index)
    {
        T* array = Data();
        return array[index];
    }
};
