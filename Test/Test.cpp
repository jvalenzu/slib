#include "slib/Test/Test.h"
#include "slib/Common/Combination.h"
#include "slib/Container/DynamicVector.h"

#include <stdio.h>

bool TestSpliceSimple();
bool TestSplice();
bool TestCombination();

bool TestSlib()
{
  if (!TestSpliceSimple())
    return false;
  if (!TestSplice())
    return false;
  if (!TestCombination())
    return false;

  return true;
}

void Dump(const DynamicVector<int>& va)
{
  for (auto v : va)
  {
    fprintf(stdout, "%d ", v);
  }
  puts("");
}

bool TestSpliceSimple()
{
  DynamicVector<int> foo({ 0, 2, 4 });
  DynamicVector<int> bar({ 0, 1, 3, 5 });

  foo.SpliceValue(2, 1, bar);

  return true;
}

bool TestCombination()
{
  const int combinations[][3] =
  {
    {0, 1, 2},
    {0, 1, 3},
    {0, 1, 4},
    {0, 1, 5},
    {0, 2, 3},
    {0, 2, 4},
    {0, 2, 5},
    {0, 3, 4},
    {0, 3, 5},
    {0, 4, 5},
    {1, 2, 3},
    {1, 2, 4},
    {1, 2, 5},
    {1, 3, 4},
    {1, 3, 5},
    {1, 4, 5},
    {2, 3, 4},
    {2, 3, 5},
    {2, 4, 5},
    {3, 4, 5},
  };

  int index = 0;
  CombinationIterator<3> itr(6);
  do
  {
    int values[3];
    itr.Values(values);

    for (int i=0; i<3; ++i)
    {
      if (values[i] != combinations[index][i])
      {
        printf("values [%d][%d][%d] != combinations(%d)[%d][%d][%d]\n",
               values[0], values[1], values[2], index,
               combinations[index][0], combinations[index][1], combinations[index][2]);
        return false;
      }
    }

    index++;
  } while(itr.Increment());
  
  return true;
}

bool TestSplice()
{
  const int values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
  for (int i=1, n=ELEMENTSOF(values); i<n; ++i)
  {
    DynamicVector<int> foo0;
    for (int j=0; j<=i; ++j)
      foo0 << values[j];

    DynamicVector<int> foo1;
    for (int j=i; j<ELEMENTSOF(values); ++j)
      foo1 << values[j];

    foo0.SpliceValue(foo1[0], i ? 1 : 0, foo1);

    int k = 0;
    for (auto v : foo0)
    {
      if (values[k] != v)
      {
        printf("%d != %d\n", values[k], v);
        return false;
      }
      k++;
    }
  }

  return true;
}
