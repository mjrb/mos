#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "types.h"

struct Chunk {
  Chunk* prev;
  Chunk* next;
  bool free;
  uint32_t size;
  // bytes for allocated space
};

struct MemManager {
  static MemManager* instance;

  Chunk* first;
  
  MemManager(void* start, unsigned size);
  void* alloc(unsigned size);
  void free(void* p);
};

void* operator new(unsigned size);
void* operator new[](unsigned size);

void* operator new(unsigned size, void* p);
void* operator new[](unsigned size, void* p);

void operator delete(void* p);
void operator delete[](void* p);


#endif
