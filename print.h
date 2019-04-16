#ifndef PRINT_H
#define PRINT_H

#define WIDTH 80
#define HEIGHT 25

#include "types.h"

void cls();
void setposc(uint32_t row, uint32_t col, char c);
extern "C" void printf(char* str);
extern "C" void printh(uint32_t p);
extern "C" void putc(char c);
// compares to strings until n
extern "C" bool strn_eq(char* str1, char* str2, uint32_t n);

#endif
