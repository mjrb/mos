#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "print.h"

#define GETC_BUF_SIZE 256

class KeyboardDriver : public InterruptHandler {
  Port8slow data;
  Port8slow com;
public:
  // fixed circular buffer/queue to store read chars
  // if we go past the max size keypresses are just droped
  static uint8_t buffer[GETC_BUF_SIZE];
  static uint32_t head;
  static uint32_t count;

  KeyboardDriver(InterruptManager* manager);
  ~KeyboardDriver();
  virtual uint32_t operator()(uint32_t esp);
};

extern "C" uint8_t getc();

// length includes \0
// garuntees buf ends with \0
extern "C" uint8_t get_line(char* buf, uint32_t length);

#endif
