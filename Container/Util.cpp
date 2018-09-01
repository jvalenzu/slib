#include "slib/Container/DynamicVector.h"
#include "slib/Container/Util.h"
#include <stdio.h>

// -------------------------------------------------------------------------------------------------
void Dump(const char* prefix, const DynamicVector<int>& vertices)
{
  printf("%-10s    ", prefix);
  for (int i=0,n=vertices.Count(); i<n; ++i)
  {
    char temp[64];
    snprintf(temp, sizeof temp, "%d ", vertices[i]);
    fputs(temp, stdout);
  }
  puts("");
}
