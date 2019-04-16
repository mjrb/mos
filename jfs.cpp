#include "jfs.h"

void File::exec() {
  ((void(*)())begin)();
}

Cursor::Cursor(File* f) : f(f), offset(0) {}

void Cursor::read(uint8_t* buf, uint32_t bytes) {
  for (int i = 0; i < bytes && offset < f->size; i++) {
    buf[i] = f->begin[offset];
    offset++;
  }
}

void Cursor::write(uint8_t* buf, uint32_t bytes) {
  for (int i = 0; i < bytes && offset < f->size; i++) {
    f->begin[offset] =  buf[i];
    offset++;
  }
}

void Cursor::seek(uint32_t offset) {
  if (offset >= f->size || offset < 0) {
    this->offset = 0;
  } else {
    this->offset = offset;
  }
}

File* JumpFS::open(char* name) {
  for (int i = 0; i < count; i++) {
    if (strn_eq(name, (char*)table[i].name, JFS_NAME_SZ)) {
      return &table[i];
    }
  }
  return 0;
}

File* JumpFS::creat(uint8_t* name, uint32_t size) {
  if (count >= capacity) {
    return 0;
  }
  
  table[count].size = size;
  for (int i = 0; i < JFS_NAME_SZ; i++) {
    table[count].name[i] = name[i];
  }
  table[count].name[JFS_NAME_SZ] = '\0';
  count++;
}

