#pragma once

#include "slib/Container/DynamicVector.h"
#include <stdint.h>
#include <stdlib.h>

template<typename KeyType> uint32_t Hash(const KeyType& value);

template<typename KeyType, typename ValueType>
class HashTable
{
  DynamicVector<KeyType> m_Keys;
  DynamicVector<ValueType> m_Values;
  DynamicVector<bool> m_Occupied; // jiv fixme
  int m_Count = 0;

  enum { kDefaultSize = 64, kMaxProbe = 8 };
    
public:
  HashTable(size_t size = kDefaultSize)
  {
    m_Keys.Reserve(size);
    m_Values.Reserve(size);
    m_Occupied.Reserve(size);
    
    m_Occupied.Fill(false);
  }

  void Insert(const KeyType& key, const ValueType& value)
  {
    uint32_t hash = Hash(key);
    const uint32_t sizemask = (m_Keys.Size()-1);
    if (m_Occupied[hash & sizemask])
    {
      hash = ~hash;
    }

    int count = 0;
    while (m_Occupied[hash & sizemask] && count < kMaxProbe)
    {
      hash++;
      count++;
    }

    assert(count != kMaxProbe);
    {
      // resize
    }

    const uint32_t index = hash & sizemask;
    m_Occupied[index] = true;
    m_Keys[index] = key;
    m_Values[index] = value;

    m_Count++;
  }

  void Discard(const KeyType& key)
  {
    size_t index;
    if (indexOf(&index, key))
    {
      m_Occupied[index] = false;
      m_Keys[index] = KeyType();
      m_Values[index] = ValueType();

      m_Count--;
    }
  }

  ValueType& operator[](const KeyType& key)
  {
    size_t index;
    if (!indexOf(&index, key))
    {
      Insert(key, ValueType());
    }

    if (indexOf(&index, key))
      return m_Values[index];

    throw std::bad_alloc();
    
  }

  bool Contains(const KeyType& key) const
  {
    size_t index;
    return indexOf(&index, key);
  }
  
  inline size_t Count() const
  {
    return m_Count;
  }

private:
  const ValueType& At(size_t index) const
  {
    assert(m_Occupied[index]);
    return m_Values[index];
  }

  bool indexOf(size_t* dindex, const KeyType& key) const
  {
    bool found = false;
    
    uint32_t hash = Hash(key);
    const uint32_t sizemask = (m_Keys.Size()-1);

    for (int i=0; !found && i<kMaxProbe; ++i)
    {
      const size_t index = (hash + i) & sizemask;
      if (m_Occupied[index] && m_Keys[index] == key)
      {
        *dindex = index;
        return true;
      }
    }
    
    hash = ~hash;
    
    for (int i=0; !found && i<kMaxProbe; ++i)
    {
      const size_t index = (hash + i) & sizemask;
      if (m_Occupied[index] && m_Keys[index] == key)
      {
        *dindex = index;
        return true;
      }
    }

    return false;
  }
};
