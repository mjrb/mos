#include "./types.h"
#include "gdt.h"
#include "port.h"
#include "print.h"
#include "interrupts.h"

extern "C" void kmain(void* multiboot_s, uint32_t magic) {
  GlobalDescriptorTable gdt;
  InterruptManager im(&gdt);
  // init hardware
  
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
  for (int i = 0; i < 23; i++) {
    printf("here\n");
  }


  while(1);
}
