#include "syscall.h"

uint32_t SYS_EAX;
uint32_t SYS_EBX;
uint32_t SYS_ECX;

SyscallHandler::SyscallHandler(InterruptManager* manager) :
  InterruptHandler(0xCD, manager) {};
SyscallHandler::~SyscallHandler() {};

uint32_t SyscallHandler::operator()(uint32_t esp) {
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
