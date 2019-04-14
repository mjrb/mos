#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "print.h"

class KeyboardDriver : public InterruptHandler {
  Port8slow data;
  Port8slow com;
public:
  KeyboardDriver(InterruptManager* manager);
  ~KeyboardDriver();
  virtual uint32_t operator()(uint32_t esp);
};

#endif
