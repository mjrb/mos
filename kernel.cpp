#include "./types.h"
#include "gdt.h"
#include "port.h"
#include "print.h"
#include "interrupts.h"
#include "keyboard.h"
#include "syscall.h"
#include "jfs.h"
#include "multiboot.h"
#include "allocator.h"


extern "C" JumpFS* testfsmeta;

extern "C" void __cxa_pure_virtual() {asm("hlt");}

extern "C" void kmain(struct multiboot_info* multiboot_s, uint32_t magic) {
  if ((multiboot_s->flags & 0x1) == 0) {
        asm("hlt"); // error can't get size of memory
  }
  MemManager mm((void*)0x1000000, multiboot_s->mem_upper);
  JumpFS testfs(testfsmeta);

  GlobalDescriptorTable gdt;
  InterruptManager im(&gdt);
  // init hardware
  KeyboardDriver kd(&im);
  SyscallHandler sh(&im, &testfs);

  im.activate();
  cls();
  printf("Welcome to my OS!\n");

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
      JFile* list = testfs.list();
      for (int i = 0; i < testfs.get_count(); i++) {
	printf((char*)list[i].get_name());
	printf("\n");
      }
      continue;
    }
    f = testfs.open(name);
    if (f == 0) {
      printf("no such file\n");
    }
    if (strn_eq(line, "cat", 3)) {
      uint8_t buf[11];
      uint8_t read = 0;
      for (int offset = 0; offset < f->get_size(); offset += 10) {
	if ((read = f->read(offset, buf, 10)) < 0) {
	  printf("error catting file\n");
	}
	buf[read] = '\0';
	printf((char*)buf);
      }
    } else if (strn_eq(line, "run", 3)) {
      // most dangerous thing in the history of operating systems
      f->exec((uint8_t*)"this was passed to program by arguments\n", 0);
    } else {
      printf("No command chose cat, run or lis\n");
    }
  }


  while(true) {
    // sleep until next interupt
    asm volatile("hlt");
  }
}
