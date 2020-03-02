#ifndef PRINT_H
#define PRINT_H

#define WIDTH 80
#define HEIGHT 25

#include "types.h"
#include "fsmanager.h"

void cls();
void setposc(uint32_t row, uint32_t col, char c);
extern "C" void printf(char* str);
extern "C" void printh(uint32_t p);
extern "C" void putc(char c);
// compares to strings until n
extern "C" bool strn_eq(char* str1, char* str2, uint32_t n);

struct ConsoleStdout : public File {
  void exec(uint8_t* args, uint32_t len) {};
  char* get_name() {return "stdout";};
  uint32_t get_size() {return 0;};
  uint32_t name_size() {return 6;};
  uint8_t read(uint32_t offset, uint8_t* buf, uint32_t bytes) {};
  uint8_t write(uint32_t offset, uint8_t* buf, uint32_t bytes);
  ConsoleStdout() {};
};

#endif
