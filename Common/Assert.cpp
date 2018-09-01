#include "slib/Common/Assert.h"

#include <execinfo.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

void FatalError(const char* msg, const char* fname, int line)
{
  printf("[%s:%d] %s\n", fname, line, msg);
  
  DumpCallstack();
  abort();
}

void DumpCallstack()
{
  const int kStackElements = 128;
  
  void* array[kStackElements];
  int num_stack_elements;
  if (num_stack_elements = backtrace(array, kStackElements))
  {
    backtrace_symbols_fd(array, num_stack_elements, STDOUT_FILENO);
  }
}
