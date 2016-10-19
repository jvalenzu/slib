// -*- mode: c++; tab-width: 4; c-basic-offset: 4; -*-
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct BitVector
{
    enum : uint32_t
    {
        kHeapAllocatedShift = 0,
        kInPlaceShift = 1
    };
    
    uint32_t m_Flags;
    size_t m_NumElements;
    uint32_t m_Data[];
    
    inline static BitVector* InitFromHeap(size_t elements)
    {
        BitVector* ret = (BitVector*) malloc(BitVector::Size(elements));
        memset(ret, 0, BitVector::Size(elements));
        ret->m_NumElements = elements;
        
        ret->m_Flags = 1<<kHeapAllocatedShift;
        return ret;
    }
    
    inline static BitVector* InitInplace(void* mem, size_t elements)
    {
        BitVector* ret = (BitVector*) mem;
        memset(ret, 0, BitVector::Size(elements));
        ret->m_NumElements = elements;
        
        ret->m_Flags = 1<<kInPlaceShift;
        
        return ret;
    }
    
    inline static size_t Size(size_t num_elements)
    {
        return sizeof(BitVector) + (num_elements>>5)*sizeof(uint32_t);
    }
    
    inline size_t Size() const
    {
        return BitVector::Size(m_NumElements);
    }
    
    inline void Reset()
    {
        memset(m_Data, 0, BitVector::Size(m_NumElements));
    }
    
    inline void Clear(size_t index)
    {
        const uint32_t word_index = index>>5;
        const uint32_t bit_index = index&31;
        m_Data[word_index] &= ~(1<<bit_index);
    }
    
    inline static void Destroy(BitVector* victim)
    {
        if (victim->m_Flags & 1<<kHeapAllocatedShift)
            delete victim;
    }
    
    inline void Set(size_t index, int value)
    {
        const uint32_t word_index = index>>5;
        const uint32_t bit_index = index&31;
        
        assert(word_index < m_NumElements);
        
        m_Data[word_index] &= ~(1<<bit_index);
        m_Data[word_index] |= (value<<bit_index);
    }
    
    inline int Get(size_t index) const
    {
        const uint32_t word_index = index>>5;
        const uint32_t bit_index = index&31;
        
        return (m_Data[word_index]>>bit_index)&1;
    }
    
    inline void Invert()
    {
        const int num_words = m_NumElements>>5;
        for (int i=0; i<num_words; ++i)
            m_Data[i] = ~m_Data[i];
    }
    
    inline void Mark(size_t index)
    {
        const uint32_t word_index = index>>5;
        const uint32_t bit_index = index&31;
        m_Data[word_index] |= ~(1<<bit_index);
    }
    
    // extract a slice of source from offset to offset+length, copy to &dest[0]
    inline void Copy(BitVector* dest, size_t offset, size_t length) const
    {
        assert(dest->m_NumElements == length);
        for (size_t i=0; i<length; ++i)
            dest->Set(i, Get(i+offset));
    }
    
    inline uint32_t* GetRaw()
    {
        return &m_Data[0];
    }
};

#define BITVECTOR_GET(bitvector, index) (((bitvector)->m_Data[((int)index)>>5] >> (((int)index)&31))&1)
