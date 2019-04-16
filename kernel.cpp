#include "./types.h"
#include "gdt.h"
#include "port.h"
#include "print.h"
#include "interrupts.h"
#include "keyboard.h"
#include "syscall.h"
#include "jfs.h"

extern "C" JumpFS* testfs;

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

#define LINE_LEN (4 + JFS_NAME_SZ + 1)
  // com file\0
  char line[LINE_LEN];
  char* name = &line[4];
  File* f = 0;
  while (true) {
    printf("COMMAND $ ");
    get_line(line, LINE_LEN);
    line[3] = '\0';

    if (strn_eq(line, "lis", 3)) {
      File* list = testfs->list();
      for (int i = 0; i < testfs->get_count(); i++) {
	printf((char*)list[i].name);
	printf("\n");
      }
      continue;
    }
    f = testfs->open(name);
    if (f == 0) {
      printf("no such file\n");
    }
    if (strn_eq(line, "cat", 3)) {
      for (int i = 0; i < f->size; i++) {
	putc((char)f->begin[i]);
      }
    } else if (strn_eq(line, "run", 3)) {
      // most dangerous thing in the history of operating systems
      f->exec();
    } else {
      printf("No command chose cat, run or lis");
    }
  }


  while(true) {
    // sleep until next interupt
    asm volatile("hlt");
  }
}
