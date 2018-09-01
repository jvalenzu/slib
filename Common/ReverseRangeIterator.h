// -*- mode: c++; tab-width: 4; c-basic-offset: 4; -*-
#pragma once

#include <stdlib.h>

#include "slib/Container/DynamicVector.h"


template<typename T>
class ReverseRangeIterator
{
  const T* m_Data;
  size_t m_Size;
  int m_Index = 0;

  struct ReverseRange
  {
    size_t m_Starti;
    size_t m_Endi;
  };

  DynamicVector<ReverseRange> m_ReverseRanges;

public:
  ReverseRangeIterator(const T* data, size_t size)
    : m_Data(data)
    , m_Size(size)
  {
  }

  template<int kDim>
  ReverseRangeIterator(const char (&data)[kDim])
    : m_Data(data)
    , m_Size(kDim-1)
  {
  }
  
  class Dereferencer
  {
    const T* m_Data;
    size_t m_Index;
    const DynamicVector<ReverseRange>& m_ReverseRanges;
    
  public:
    Dereferencer(const T* data,
                 size_t index,
                 const DynamicVector<ReverseRange>& reverseRanges)
      : m_Data(data)
      , m_Index(index)
      , m_ReverseRanges(reverseRanges)
    {
    }

    inline bool operator != (const Dereferencer& rhs)
    {
      return m_Index != rhs.m_Index;
    }

    inline Dereferencer& operator ++()
    {
      m_Index++;
      return *this;
    }

    inline const T& operator*() const
    {
      size_t reverse_interval_start = 0;
      size_t reverse_interval_end = 0;
      for (ReverseRange r : m_ReverseRanges)
      {
        if (r.m_Starti <= m_Index && m_Index <= r.m_Endi)
        {
          reverse_interval_start = r.m_Starti;
          reverse_interval_end = r.m_Endi;
          break;
        }
      }

      size_t index = m_Index;
      if (reverse_interval_start != reverse_interval_end)
      {
        index = reverse_interval_end - m_Index + reverse_interval_start;
      }

      return m_Data[index];
    }
  };

  const Dereferencer begin() const
  {
    return Dereferencer(m_Data, 0, m_ReverseRanges);
  }

  const Dereferencer end() const
  {
    return Dereferencer(m_Data, m_Size, m_ReverseRanges);
  }

  void addReverseRange(size_t starti, size_t endi)
  {
    ReverseRange* r = m_ReverseRanges.Alloc(1);
    *r = ReverseRange{starti, endi};
  }

  size_t Count() const
  {
    return m_Size;
  }
};
