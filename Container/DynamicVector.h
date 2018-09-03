// -*- mode: c++; tab-width: 4; c-basic-offset: 4; -*-

#pragma once

#include "slib/Common/Assert.h"
#include "slib/Common/Util.h"
#include "slib/Container/Private/ObjectConstructor.h"

#include <initializer_list>
#include <new>
#include <string.h>
#include <stdio.h>

template <typename T>
class DynamicVector
{
    char* m_Data = nullptr;
    int m_Size = 0;
    int m_Count = 0;

    enum { kNumElementsBuffer = 64 };

public:
    typedef T Type;
    
    DynamicVector()
    {
    }

    template<std::size_t N>
    DynamicVector(T (&data)[N])
    {
        for (auto i : data)
        {
            *this << i;
        }
    }

    DynamicVector(std::initializer_list<T> list)
    {
        for (auto i : list)
        {
            *this << i;
        }
    }

    DynamicVector(DynamicVector&& rhs)
    {
        m_Data = rhs.m_Data;
        m_Size = rhs.m_Size;
        m_Count = rhs.m_Count;

        rhs.m_Data = nullptr;
        rhs.m_Size = 0;
        rhs.m_Count = 0;
    }

    DynamicVector(const DynamicVector& rhs)
    {
        Alloc(rhs.m_Count);

        int i = 0;
        for (auto value : rhs)
        {
            (*this).operator[](i++) = value;
        }
    }
    
    DynamicVector(size_t num_elements)
    {
        (void)Alloc(num_elements);
    }
    
    ~DynamicVector()
    {
        delete [] m_Data;
    }


    DynamicVector<T>& operator=(DynamicVector<T>& rhs)
    {
        m_Data = rhs.m_Data;
        m_Count = rhs.m_Count;
        m_Size = rhs.m_Size;

        rhs.m_Data = nullptr;
        rhs.m_Count = 0;
        rhs.m_Size = 0;
        
        return *this;
    }

    DynamicVector<T>& operator=(const DynamicVector<T>& rhs)
    {
        ssize_t diff = rhs.Count() - Count();
        if (diff > 0)
            Alloc(diff);

        int i = 0;
        for (auto value : rhs)
        {
            (*this).operator[](i++) = value;
        }
        return *this;
    }
    
    T* Data()
    {
        return (T*)m_Data;
    }
    
    const T* Data() const
    {
        return (T*)m_Data;
    }

    void Clear()
    {
        m_Count = 0;
    }
    
    inline int Count() const
    {
        return m_Count;
    }

    size_t Size() const
    {
        return m_Size;
    }

    void Reserve(size_t size)
    {
        if (size > m_Size)
        {
            const size_t diff = size - m_Size;
            Alloc(diff);
        }
    }
    
    T* Alloc(size_t num_elements=1)
    {
        if (m_Count + num_elements > m_Size)
        {
            const size_t next_num_elements = (m_Count + num_elements + kNumElementsBuffer - 1) & ~(kNumElementsBuffer-1);
            assert(next_num_elements >= m_Size);
            
            char* temp = new char [sizeof(T) * next_num_elements];
            if (temp == nullptr)
                return nullptr;
            memcpy(temp, m_Data, m_Size * sizeof(T));
            
            delete [] m_Data;
            m_Data = temp;

            m_Size = next_num_elements;
        }
        
        T* ret = (T*) (m_Data + sizeof(T) * m_Count);
        for (int i=0; i<num_elements; ++i)
        {
            ObjectConstructor<T, std::is_default_constructible<T>::value> o(&ret[i]);
        }
        m_Count += num_elements;
        return ret;
    }

    void Steal(T* data, size_t num_elements)
    {
        delete [] m_Data;
        m_Data = (char*) data;
        m_Count = num_elements;
        m_Size = num_elements;
    }
    
    int IndexOf(const T* data)
    {
        const size_t index = data - Data();
        if (index <0 || index >= m_Count)
            return -1;
        return (int) index;
    }
    
    void Erase(T* victim)
    {
        const size_t index = victim - Data();
        Erase(index);
    }
    
    void Erase(size_t index)
    {
        if (index < m_Count)
        {
            T* victim = Data() + index;
            *victim = *(Data() + --m_Count);
        }
    }

    void EraseInPlace(T* victim)
    {
        const size_t index = victim - Data();
        EraseInPlace(index);
    }
    
    void EraseInPlace(size_t index)
    {
        if (index >= m_Count)
            return;
        const int next_count = --m_Count;
        if (index == next_count)
            return;

        size_t last = next_count-index;
        for (int i=0; i<last; ++i)
        {
            (*this).operator[](index + i) = (*this).operator[](index + i + 1);
        }
    }

    T* Last()
    {
        if (m_Count == 0)
            return nullptr;
        return &(*this).operator[](m_Count-1);
    }

    T* Pop()
    {
        if (m_Count == 0)
            return nullptr;
        
        return (T*) (m_Data + sizeof(T) * --m_Count);
    }

    T PopFront()
    {
        if (m_Count == 0)
        {
            throw std::bad_alloc();
            return T();
        }
        
        T ret = (*this).operator[](0);
        EraseInPlace(size_t(0));
        return ret;
    }

    void Append(const T& value)
    {
        if (T* dst = Alloc())
        {
            *dst = value;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void Append(const DynamicVector<T>& values)
    {
        for (auto value : values)
        {
            Append(value);
        }
    }

    void SpliceValue(const T& value, size_t to_remove, const DynamicVector<T>& rhs)
    {
        size_t index = 0;
        while (this->operator[](index) != value)
        {
            index++;
        }

        if (index == Count())
            return;

        const size_t num_elements = rhs.Count() - to_remove;
        const size_t offset = Count() - to_remove - 1;
        Alloc(num_elements);

        // jiv fixme: memmove for POD types
        for (int i=offset; i>=0; --i)
        {
            (*this).operator[](index + i + num_elements) = (*this).operator[](index + i);
        }
        
        T* values = Data() + index;
        for (auto v : rhs)
        {
            *values++ = v;
        }
    }
    
    T operator[](size_t index) const
    {
        assert(index >= 0);
        assert(index < Count());
            
        return *(T*)(m_Data + sizeof(T)*index);
    }

    T& operator[](size_t index)
    {
        assert(index >= 0);
        assert(index < Size());
        return *At(index);
    }

    T* At(size_t index)
    {
        return (T*)(m_Data + sizeof(T)*index);
    }
    
    DynamicVector<T>& operator<<(const T& v)
    {
        Append(v);
        return *this;
    }

    void Fill(const T& value)
    {
        for (auto& i : *this)
        {
            i = value;
        }
    }

    class Iterator
    {
        DynamicVector<T>& m_Parent;
        size_t m_Index;

    public:
        Iterator(DynamicVector<T>& parent, size_t index)
            : m_Parent(parent)
            , m_Index(index)
        {
        }

        operator T*()
        {
            return m_Parent.At(m_Index);
        }

        // preincrement
        Iterator& operator++()
        {
            ++m_Index;
            return *this;
        }
        
        Iterator operator++(int)
        {
            Iterator temp(m_Parent, m_Index);
            ++m_Index;
            return temp;
        }
    };

    class CycleIterator
    {
        DynamicVector<T>& m_Parent;
        size_t m_Index;

    public:
        CycleIterator(DynamicVector<T>& parent, size_t index)
            : m_Parent(parent)
            , m_Index(index)
        {
        }

        operator T*()
        {
            return m_Parent.At(m_Index);
        }

        // preincrement
        CycleIterator& operator++()
        {
            if (++m_Index >= m_Parent.Count())
            {
                m_Index = 0;
            }

            return *this;
        }
        
        CycleIterator operator++(int)
        {
            CycleIterator temp(m_Parent, m_Index);
            if (++m_Index >= m_Parent.Count())
            {
                m_Index = 0;
            }
            return temp;
        }
    };

    class ReverseIterator
    {
        DynamicVector<T>& m_Parent;
        size_t m_Index;

    public:
        ReverseIterator(DynamicVector<T>& parent, size_t index)
            : m_Parent(parent)
            , m_Index(index)
        {
        }

        operator T*()
        {
            return m_Parent.At(m_Index);
        }

        // preincrement
        ReverseIterator& operator++()
        {
            if (--m_Index >= m_Parent.Count())
            {
                m_Index = m_Parent.Count()-1;
            }

            return *this;
        }

        ReverseIterator operator++(int)
        {
            ReverseIterator temp(m_Parent, m_Index);
            if (--m_Index >= m_Parent.Count())
            {
                m_Index = m_Parent.Count()-1;
            }
            return temp;
        }
    };

    T* begin()
    {
        return At(0);
    }

    T* end()
    {
        return At(m_Count);
    }
    
    const T* begin() const
    {
        return (const T*) m_Data;
    }

    const T* end() const
    {
        return (const T*) (m_Data + m_Count * sizeof(T));
    }

    ReverseIterator ReverseIteratorAt(size_t index) const
    {
        return ReverseIterator(*this, index);
    }

    explicit operator bool() const
    {
        return Count()>0;
    }

    void CopyFrom(const DynamicVector<T>& rhs)
    {
        assert(Size() >= rhs.Count());
        const int add_elements = rhs.Count() - Count();
        if (add_elements > 0)
            Alloc(add_elements);

        int i = 0;
        for (auto j : rhs)
        {
            (*this)[i++] = j;
        }
    }
};

void Dump(const char* prefix, const DynamicVector<int>& just);
