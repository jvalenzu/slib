#include "slib/Container/Hash.h"

template<>
uint32_t Hash<int>(const int& value)
{
  return value;
}
