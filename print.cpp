#include "print.h"

uint16_t* buffer = (uint16_t*)0xb8000;
int linecount = 0;
int colcount = 0;

void cls() {
  for (uint32_t i = 0; i < HEIGHT*WIDTH; i++) {
    buffer[i] &= 0xFF00;
  }
  linecount = 0;
  colcount = 0;
}
  
void setposc(uint32_t row, uint32_t col, char c) {
  buffer[row * WIDTH + col] = (buffer[row * WIDTH + col] & 0xFF00) | c;
}

void newline() {
  linecount++;
  colcount = 0;
  if (linecount >= HEIGHT) {
    for (int i = 0; i < HEIGHT - 1; i++) {
      for (int j = 0; j < WIDTH; j++) {
	buffer[i * WIDTH + j] = buffer[(i + 1) * WIDTH + j];
      }
    }
    linecount = HEIGHT -1;
    for (int j = 0; j < WIDTH; j++) {
      setposc(HEIGHT-1, j, ' ');
    }
  }
}

extern "C" void printf(char* str) {
  for(uint32_t i = 0; str[i] != '\0'; i++) {
    if (str[i] == '\n') {
      newline();
    } else {
      setposc(linecount, colcount, str[i]);
      colcount++;
      if (colcount >= WIDTH) {
	newline();
      }
    }
  }
}

char nib2char(uint8_t nib) {
  nib &= 0xF;
  if (0 <= nib && nib <= 9) {
    return nib + '0';
  } else {
    return nib - 9 + 'A';
  }
}

extern "C" void printh(uint32_t p) {
  char buf[11] = "0x00000000";
  for (int i = 9; i > 1; i--) {
    buf[i] = nib2char(p);
    p = p >> 4;
  }
  printf(buf);
}

