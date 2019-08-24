#include "fsmanager.h"

void File::exec(uint8_t* args, uint32_t len) {
  auto space = new uint8_t[get_size()];
  uint8_t nread = read(0, space, get_size());
  ((void(*)(uint8_t*, uint32_t))space)(args, len);
  delete space;
}

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
