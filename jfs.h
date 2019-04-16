#ifndef JFS_H
#define JFS_H

// excluding null term
#define JFS_NAME_SZ 5

#include "print.h"
#include "types.h"

struct File {
  uint8_t name[JFS_NAME_SZ + 1];
  uint32_t size;
  uint8_t* begin;
  void exec();
}__attribute((packed));

class Cursor {
  File* f;
  uint32_t offset;
public:
  Cursor(File* f);
  void read(uint8_t* buf, uint32_t bytes);
  void write(uint8_t* buf, uint32_t bytes);
  void seek(uint32_t offset);
};

class JumpFS {
  uint32_t capacity;
  uint32_t count;
  File* table;
public:
  File* open(char* name);
  File* creat(uint8_t* name, uint32_t size);
  uint32_t get_count() {return count;};
  uint32_t get_capacity() {return capacity;};
  File* list() {return table;};
}__attribute((packed));

#endif
