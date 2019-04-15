#include "./types.h"
#include "gdt.h"
#include "port.h"
#include "print.h"
#include "interrupts.h"
#include "keyboard.h"
#include "syscall.h"

// compares to strings until n
bool strn_eq(char* str1, char* str2, uint32_t n) {
    bool eq = true;
    for (int i = 0; i < n; i++) {
      if (str1[i] != str2[i]) {
	eq = false;
	break;
      }
    }
    return eq;
}


extern "C" void program();
extern "C" void dprogram();

extern "C" void kmain(void* multiboot_s, uint32_t magic) {
  GlobalDescriptorTable gdt;
  InterruptManager im(&gdt);
  // init hardware
  KeyboardDriver kd(&im);
  SyscallHandler sh(&im);

  im.activate();
  cls();
  printf("Welcome to my OS!\n");
  printf("test");
  printf(" part 2\n");
  printh((uint32_t)gdt.code_offset());
  printf("\n");

  //trace bounds
  setposc(0, WIDTH - 1, 'a');
  setposc(HEIGHT - 1, 0, 'b');
  setposc(HEIGHT - 1, WIDTH - 1, 'c');

  //test scroll
  for (int i = 0; i < 20; i++) {
    printf("here\n");
  }

  char buf[80];
  while (true) {
    printf("type some text: ");
    get_line(buf, 80);
    if (strn_eq(buf, "program\n", 9)) {
      printf("running program\n");
      program();
    } else if (strn_eq(buf, "dprogram\n", 10)) {
      printf("running program from data section\n");
      dprogram();
    } else {
      printf("recieved line: ");
      printf(buf);
    }
  }


  while(true) {
    // sleep until next interupt
    asm volatile("hlt");
  }
}
