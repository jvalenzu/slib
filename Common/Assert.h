#pragma once

#define assert(x) do { if (!(x)) FatalError(#x, __FILE__, __LINE__); } while(0)
#define SSSERT(x) do { if (!(x)) FatalError(#x, __FILE__, __LINE__); } while(0)

void FatalError(const char* msg, const char* fname, int line);
void DumpCallstack();
