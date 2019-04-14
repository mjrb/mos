#ifndef PRINT_H
#define PRINT_H

#define WIDTH 80
#define HEIGHT 25

#include "types.h"

void cls();
void setposc(uint32_t row, uint32_t col, char c);
extern "C" void printf(char* str);
extern "C" void printh(uint32_t p);
#endif
