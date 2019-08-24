#include "jfs.h"

char* JFile::get_name() {return (char*)meta->name;}
uint32_t JFile::get_size() {return meta->size;}
uint32_t JFile::name_size() {return JFS_NAME_SZ + 1;}


uint8_t JFile::read(uint32_t offset, uint8_t* buf, uint32_t bytes) {
  int i = 0;
  for (; i < bytes && offset < this->meta->size; i++) {
    buf[i] = this->meta->begin[offset];
    offset++;
  }
  return i;
}

uint8_t JFile::write(uint32_t offset, uint8_t* buf, uint32_t bytes) {
  for (int i = 0; i < bytes && offset < this->meta->size; i++) {
    this->meta->begin[offset] =  buf[i];
    offset++;
  }
  return 0;
}

JumpFS::JumpFS(JumpFS* meta) {
  *this = *meta;
  table = new JFile[capacity];
  for (int i; i < capacity; i++) {
    table[i].meta = &metable[i];
  }
}

JumpFS::~JumpFS() {
  delete table;
}


File* JumpFS::open(char* name) {
  for (int i = 0; i < count; i++) {
    if (strn_eq(name, (char*)table[i].meta->name, JFS_NAME_SZ)) {
      return &table[i];
    }
  }
  return 0;
}

File* JumpFS::creat(uint8_t* name, uint32_t size) {
  if (count >= capacity) {
    return 0;
  }
  
  table[count].meta->size = size;
  for (int i = 0; i < JFS_NAME_SZ; i++) {
    table[count].meta->name[i] = name[i];
  }
  table[count].meta->name[JFS_NAME_SZ] = '\0';
  count++;
}

uint32_t JumpFS::get_count() {return count;}
uint32_t JumpFS::get_capacity() {return capacity;}
JFile* JumpFS::list() {return table;}
