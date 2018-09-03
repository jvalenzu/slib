// -*- mode: c++; tab-width: 4; c-basic-offset: 4; -*-
#pragma once

#include <stdlib.h>

#include "slib/Container/FixedVector.h"

// Shift and Reverse Iterator
template<typename T, int maxSegments=4> class SarIteratorWrapper
{
  const T* m_Data;
  size_t m_Size;
  int m_Index = 0;

public:
  enum { kMaxSegments = maxSegments };
  struct Segment
  {
    size_t m_Start;
    size_t m_Endi;
    bool m_Reversed;

    Segment(size_t start, size_t endi, bool reversed)
      : m_Start(start)
      , m_Endi(endi)
      , m_Reversed(reversed)
    {
    }

    size_t Count() const
    {
      return m_Endi - m_Start + 1;
    }
  };
private:
  FixedVector<Segment, maxSegments> m_Segments;
  int m_SegmentItr = 0;

public:
  SarIteratorWrapper(const T* data, size_t size)
    : m_Data(data)
    , m_Size(size)
  {
  }

  template<int kDim>
  SarIteratorWrapper(const char (&data)[kDim])
    : m_Data(data)
    , m_Size(kDim-1)
  {
  }
  
  class SarIterator
  {
  public:
    const T* m_Data;
    int m_SegmentItr;
    size_t m_Index;
    const FixedVector<Segment, maxSegments>& m_Segments;
    
  public:
    SarIterator(const T* data,
                int segment_itr,
                size_t index,
                const FixedVector<Segment, maxSegments>& segments)
      : m_Data(data)
      , m_SegmentItr(segment_itr)
      , m_Index(index)
      , m_Segments(segments)
    {
    }

    inline bool operator != (const SarIterator& rhs)
    {
      return m_Index != rhs.m_Index || m_SegmentItr != rhs.m_SegmentItr;
    }

    inline SarIterator& operator ++()
    {
      const Segment& segment = m_Segments[m_SegmentItr];
      if (++m_Index >= segment.Count())
      {
        m_Index = 0;
        m_SegmentItr++;
      }
      return *this;
    }

    inline const T& operator*() const
    {
      const Segment& segment = m_Segments[m_SegmentItr];

      size_t index = m_Index;
      if (!segment.m_Reversed)
      {
        index = segment.m_Start + m_Index;
      }
      else
      {
        index = segment.m_Endi - m_Index;
      }

      return m_Data[index];
    }
  };

  const SarIterator begin() const
  {
    return SarIterator(m_Data, m_SegmentItr, 0, m_Segments);
  }

  const SarIterator end() const
  {
    return SarIterator(m_Data, m_Segments.Count(), 0, m_Segments);
  }

  T operator[](size_t index) const
  {
    int i=0;
    for (int n=m_Segments.Count(); i<n; ++i)
    {
      if (index < m_Segments[i].Count())
      {
        break;
      }

      index -= m_Segments[i].Count();
    }
    return *SarIterator(m_Data, i, index, m_Segments);
  }

  int segmentAt(size_t index) const
  {
    SarIterator i = begin();
    while (index--)
    {
      ++i;
    }
    return i.m_SegmentItr;
  }

  void addSegment(size_t starti, size_t endi, bool reversed)
  {
    Segment* s = m_Segments.Alloc(1);
    s->m_Start = starti;
    s->m_Endi = endi;
    s->m_Reversed = reversed;
  }

  void addSegment(const Segment& s)
  {
    if (s.Count() == 0)
    {
      return;
    }

    *m_Segments.Alloc() = s;
  }

  size_t Count() const
  {
    return m_Size;
  }

  template<typename Container>
  void FlattenInto(Container& c)
  {
    assert(c.Count() <= Count());
    
    int i = 0;
    for (auto v : *this)
    {
      c[i++] = v;
    }
  }
};
