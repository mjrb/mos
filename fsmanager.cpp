#include "fsmanager.h"

void FSManager::mount(char name, FileSystem* fs) {
  if (count < FS_MAX) {
    fss[count].fs = fs;
    fss[count].name = name;
    count++;
  }
}

FileSystem* FSManager::get_fs(char* path, uint32_t pathlen) {
  if (pathlen > 0 && path[0] != '\0') {
    for (int i = 0; i < count; i++) {
      if (fss[i].name = path[0]) {
	return fss[i].fs;
      }
    }
  }
  return 0;
}

File* FSManager::open(char* path, uint32_t pathlen) {
  if (pathlen > 2) {
    auto fs = get_fs(path, pathlen);
    return fs->open(path + 2);
  }
  return 0;
}
