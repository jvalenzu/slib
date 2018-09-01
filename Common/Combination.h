#pragma once

#include <stdlib.h>
#include <string.h>

template <int K>
class CombinationIterator
{
  int m_N;
  int m_State[K];
public:
  CombinationIterator(size_t n)
    : m_N(n)
    , m_State{0}
  {
    for (size_t i=0; i<K; ++i)
    {
      m_State[i] = i;
    }
  }

  void Values(int* values) const
  {
    memcpy(values, m_State, K*sizeof(int));
  }

  void SetValues(int* values)
  {
    memcpy(m_State, values, K*sizeof(int));
  }

  bool Increment()
  {
    int itr = K-1;

    if (m_State[itr] == m_N-1)
    {
      --itr;
      while (itr > 0 && m_State[itr] == m_State[itr+1]-1)
        itr--;
    }

    if (itr <= 0)
      return false;

    m_State[itr]++;

    while (++itr <= K-1)
    {
      m_State[itr] = m_State[itr-1]+1;
    }

    return true;
  }
};
