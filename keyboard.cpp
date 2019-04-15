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

uint8_t KeyboardDriver::buffer[GETC_BUF_SIZE];
uint32_t KeyboardDriver::head = 0;
uint32_t KeyboardDriver::count = 0;

#define KMAP(code, c) case code: key = c; break;

uint32_t KeyboardDriver::operator()(uint32_t esp) {
  uint8_t key = data.read();
  switch(key) {
    KMAP(0x2, '1');
    KMAP(0x3, '2');
    KMAP(0x4, '3');
    KMAP(0x5, '4');
    KMAP(0x6, '5');
    KMAP(0x7, '6');
    KMAP(0x8, '7');
    KMAP(0x9, '8');
    KMAP(0xA, '9');
    KMAP(0xB, '0');
    KMAP(0xE, '\b');

    KMAP(0x10, 'q');
    KMAP(0x11, 'w');
    KMAP(0x12, 'e');
    KMAP(0x13, 'r');
    KMAP(0x14, 't');
    KMAP(0x15, 'y');
    KMAP(0x16, 'u');
    KMAP(0x17, 'i');
    KMAP(0x18, 'o');
    KMAP(0x19, 'p');

    KMAP(0x1E, 'a');
    KMAP(0x1F, 's');
    KMAP(0x20, 'd');
    KMAP(0x21, 'f');
    KMAP(0x22, 'g');
    KMAP(0x23, 'h');
    KMAP(0x24, 'j');
    KMAP(0x25, 'k');
    KMAP(0x26, 'l');
    KMAP(0x1C, '\n');

    KMAP(0x2C, 'z');
    KMAP(0x2D, 'x');
    KMAP(0x2E, 'c');
    KMAP(0x2F, 'v');
    KMAP(0x30, 'b');
    KMAP(0x31, 'n');
    KMAP(0x32, 'm');

    KMAP(0x39, ' ');
  default:
    if (key > 0x80) {
      // key up code
      return esp;
    }
    key = '?';
  }
  if (count < GETC_BUF_SIZE) {
    uint32_t index = (head + count) % GETC_BUF_SIZE;
    buffer[index] = key;
    count++;
  }
  return esp;
}

extern "C" uint8_t getc() {
  while (KeyboardDriver::count == 0) {
    // start interrupts and wait for a char
    asm volatile("sti\n"
		 "hlt");
  }
  asm volatile("cli");
  uint8_t result = KeyboardDriver::buffer[KeyboardDriver::head];
  KeyboardDriver::head++;
  KeyboardDriver::count--;
  asm volatile("sti");
  return result;
}

extern "C" uint8_t get_line(char* buf, uint32_t length) {
  uint8_t c = getc();
  int i = 0;
  while (c != '\n' && i < length - 1) {
    if (c == '\b') {
      // only backspace until begining of buffer
      if (i > 0) {
	putc('\b');
	i--;
      }
      c = getc();
    } else {
      putc(c);
      buf[i] = c;
      c = getc();
      i++;
    }
  }
  // if the last char was \n insert it
  // if not we line ran out of buffer
  if (c == '\n') {
    putc('\n');
    if (i < length - 1) {
      buf[i] = '\n';
      i++;
    }
  }
  // insert nul term
  if (i < length) {
    buf[i] = '\0';
  }
}
