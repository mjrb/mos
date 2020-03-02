#include "syscall.h"

uint32_t SYS_EAX;
uint32_t SYS_EBX;
uint32_t SYS_ECX;
uint32_t SYS_EDX;
uint32_t SYS_ESI;
uint32_t SYS_EDI;

SyscallHandler::SyscallHandler(InterruptManager* manager, JumpFS* jfs) :
  InterruptHandler(0xCD, manager), jfs(jfs) {};
SyscallHandler::~SyscallHandler() {};

uint32_t SyscallHandler::operator()(uint32_t esp) {
  static File* open_files[255];
  static uint16_t next = 2;
  static ConsoleStdout stdout;
  static KeyboardStdin stdin;

  open_files[0] = &stdin;
  open_files[1] = &stdout;

  switch (SYS_EAX) {
  case 0:
    // exit
    break;
  case 1:
    // read
    open_files[SYS_EBX]->read(SYS_ECX, (uint8_t*)SYS_EDI, SYS_EDX);
    break;
  case 2:
    // write
    open_files[SYS_EBX]->write(SYS_ECX, (uint8_t*)SYS_ESI, SYS_EDX);
    break;
  case 3:
    // open
    open_files[next] = jfs->open((char*)SYS_ESI);
    if (open_files[next] == 0) {
      SYS_EAX = 0xFF; // -1
      break;
    }
    SYS_EAX = next;
    next++;
  case 4:
    // exec
    open_files[SYS_EBX]->exec((uint8_t*)SYS_ESI, SYS_ECX);
    break;
  case 5:
    // close
  default:
    printf("REG ");
    printh(SYS_EAX);
    printf(" ");
    printh(SYS_EBX);
    printf(" ");
    printh(SYS_ECX);
    printf("\n");
    printh(SYS_EDX);
    printf(" ");
    printh(SYS_ESI);
    printf(" ");
    printh(SYS_EDI);
    printf("\n");
  }
  return esp;
}
