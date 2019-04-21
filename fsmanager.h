#ifndef FSMANAGER_H
#define FSMANAGER_H

#include "types.h"

struct File {
  virtual void exec(uint8_t* args, uint32_t len) = 0;
  virtual char* get_name();
  virtual uint32_t get_size() = 0;
  virtual uint32_t name_size() = 0;
  virtual uint8_t read(uint32_t offset, uint8_t* buf, uint32_t bytes) = 0;
  virtual uint8_t write(uint32_t offset, uint8_t* buf, uint32_t bytes) = 0;
protected:
  File() {};
};

class FileSystem {
protected:
  FileSystem() {};
public:
  virtual File* open(char* name) = 0;
  virtual File* creat(uint8_t* name, uint32_t size) = 0;
  virtual File* list() = 0;
};

#define FS_MAX 10
struct FSManager {
  struct entry {
    FileSystem* fs = 0;
    char name = '\0';
  };

  entry fss[FS_MAX];
  uint32_t count;

  FileSystem* get_fs(char* path, uint32_t pathlen);
  //paths are A/file
  File* open(char* path, uint32_t pathlen);
  void mount(char name, FileSystem* fs);
  
};

#endif
