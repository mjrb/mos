#include "keyboard.h"

KeyboardDriver::KeyboardDriver(InterruptManager* manager) :
  InterruptHandler(0x21, manager), data(0x60), com(0x64)
{
  while(com.read() & 0x1) {
    data.read();
  }
  // tell keybaord to use interrupts
  com.write(0xAE);
  
  // change state to ??
  // get current state
  com.write(0x20);
  uint8_t state = (data.read() | 1) & ~0x10;
  // set state
  com.write(0x60);
  data.write(state);

  // activate keyboard
  data.write(0xF4);
}

KeyboardDriver::~KeyboardDriver() {
  
}

uint32_t KeyboardDriver::operator()(uint32_t esp) {
  uint8_t key = data.read();
  printf("keypress ");
  printh(key);
  printf("\n");
  return esp;
}
