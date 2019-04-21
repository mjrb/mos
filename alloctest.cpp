#include "allocator.h"
#include "types.h"

uint8_t heap[65536];

template<typename T>
struct unq {
  T* val;

  unq(T* val) : val(val) {}
  ~unq() {delete val;}
  
  T operator*() {return *val;}
};

struct D {
  int f;
};

int main(int argc, char** argv) {
  MemManager mm((void*)heap, 65536);
  D* d1 = new D();
  D* d2 = new D();
  D* d3 = new D();
  D* d4 = new D();
  D* d5 = new D();
  D* d6 = new D();
  D* d7 = new D();

  // nofree
  delete d2;
  // free after
  delete d1;
  // free before
  delete d3;

  delete d5;
  // free before and after
  delete d4;
  
  return 0;
}
