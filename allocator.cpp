#include "allocator.h"

MemManager* MemManager::instance = 0;

MemManager::MemManager(void* start, unsigned size) {
  instance = this;
  
  first = (Chunk*)start;
  first->prev = 0;
  first->next = 0;
  first->size = size - sizeof(Chunk);
  first->free = true;
}

void* MemManager::alloc(unsigned size) {
  Chunk* result = first;

  // find somthing free and big enough
  while (!(result->free && result->size >= size + sizeof(Chunk))) {
    if (result->next == 0) {
      return 0;
    }
    result = result->next;
  }

  // make a new free space after result
  Chunk* new_next = (Chunk*)((uint8_t*)result + sizeof(Chunk) + size);
  new_next->next = result->next;
  new_next->prev = result;
  new_next->free = true;
  new_next->size = result->size - sizeof(Chunk) - size;

  // change result to our new allocated space
  result->next = new_next;
  result->free = false;
  result->size = size;
  
  return (uint8_t*)result + sizeof(Chunk);
}

void MemManager::free(void* p) {
  Chunk* c = (Chunk*)((uint8_t*)p - sizeof(Chunk));
  c->free = true;

  // defrag forward
  if (c->next != 0 && c->next->free) {
    c->size += c->next->size + sizeof(Chunk);
    c->next = c->next->next;
    c->next->prev = c;
  }

  // defrag backward
  if (c->prev != 0 && c->prev->free) {
    Chunk* prev = c->prev;
    prev->size += c->size + sizeof(Chunk);
    prev->next = c->next;
    prev->next->prev = prev;
  }
}

void* operator new(unsigned size) {
  if (MemManager::instance == 0) return 0;
  return MemManager::instance->alloc(size);
}

void* operator new[](unsigned size) {
  if (MemManager::instance == 0) return 0;
  return MemManager::instance->alloc(size);
}

void* operator new(unsigned size, void* p) {return p;}
void* operator new[](unsigned size, void* p) {return p;}

void kdelete(void* p) {
  if (MemManager::instance != 0)
    return MemManager::instance->free(p);
}
void operator delete(void* p) {
  return kdelete(p);
}

void operator delete[](void* p) {
  return kdelete(p);
}

void operator delete(void* p, unsigned size) {
  return kdelete(p);
}

void operator delete[](void* p, unsigned size) {
  return kdelete(p);
}
