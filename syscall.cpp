#include "syscall.h"

uint32_t SYS_EAX;
uint32_t SYS_EBX;
uint32_t SYS_ECX;
uint32_t SYS_EDX;

SyscallHandler::SyscallHandler(InterruptManager* manager, JumpFS* jfs) :
  InterruptHandler(0xCD, manager), jfs(jfs) {};
SyscallHandler::~SyscallHandler() {};

uint32_t SyscallHandler::operator()(uint32_t esp) {
  static File* open_files[255];
  static uint16_t next = 0;

  switch (SYS_EAX) {
  case 0:
    // exit
    break;
  case 1:
    // printf
    printf((char*) SYS_EBX);
    break;
  case 2:
    putc((char)SYS_EBX);
    break;
  case 3:
    // getc
    SYS_EAX = getc();
    break;
  case 4:
    open_files[next] = jfs->open((char*)SYS_EBX);
    if (open_files[next] == 0) {
      SYS_EAX = 0xFF; // -1
      break;
    }
    SYS_EAX = next;
    next++;
  case 5:
    open_files[SYS_EAX]->exec((uint8_t*)SYS_ECX, SYS_EBX);
    break;
  default:
    printf("REG ");
    printh(SYS_EAX);
    printf(" ");
    printh(SYS_EBX);
    printf(" ");
    printh(SYS_ECX);
    printf("\n");
  }
  return esp;
}
