#ifndef JFS_H
#define JFS_H

// excluding null term
#define JFS_NAME_SZ 5

#include "print.h"
#include "types.h"
#include "fsmanager.h"

struct JCursor;

struct JFATEntry {
  uint8_t name[JFS_NAME_SZ + 1];
  uint32_t size;
  uint8_t* begin;
}__attribute((packed));

struct JFile : public File {
  JFATEntry* meta;
  JFile() {};
  char* get_name();
  uint32_t get_size();
  uint32_t name_size();
  uint8_t read(uint32_t offset, uint8_t* buf, uint32_t bytes);
  uint8_t write(uint32_t offset, uint8_t* buf, uint32_t bytes);
};

class JumpFS{// : public FileSystem
  uint32_t capacity;
  uint32_t count;
  JFATEntry* metable;
  JFile* table;
public:
  ~JumpFS();
  JumpFS(JumpFS* meta);
  File* open(char* name);
  File* creat(uint8_t* name, uint32_t size);
  uint32_t get_count();
  uint32_t get_capacity();
  JFile* list();
}__attribute((packed));

#endif
