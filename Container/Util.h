#pragma once

#include <stdlib.h>
#include <stdint.h>


template <typename ContainerType> size_t RandomIndex(ContainerType* container)
{
  const size_t count = (1+container->Count()) * (rand() / (1.0f * RAND_MAX));
  return count;
}

template <typename ContainerType> size_t RandomIndex(const ContainerType& container)
{
  const size_t count = (1+container.Count()) * (rand() / (1.0f * RAND_MAX));
  return count;
}

template <typename ContainerType>
typename ContainerType::Type RandomChoicePop(ContainerType* container)
{
  const size_t count = RandomIndex(container);
  auto ret = container->operator[](count);
  container->EraseInPlace(count);
  return ret;
}

template <typename ContainerType>
typename ContainerType::Type RandomChoice(ContainerType* container)
{
  const size_t count = RandomIndex(container);
  return container->operator[](count);
}

template <typename ContainerType>
typename ContainerType::Type RandomChoice(const ContainerType& container)
{
  const size_t count = RandomIndex(container);
  return container[count];
}

// -------------------------------------------------------------------------------------------------
template <typename ContainerType, typename L>
size_t Count(const ContainerType& container, L lambda)
{
  size_t ret = 0;
  for (auto x : container)
  {
    if (lambda(x))
      ret++;
  }
  return ret;
}
