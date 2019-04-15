#ifndef SYSCALL_H
#define SYSCALL_H

#include "types.h"
#include "interrupts.h"
#include "print.h"
#include "keyboard.h"

extern uint32_t SYS_EAX;
extern uint32_t SYS_EBX;
extern uint32_t SYS_ECX;

class SyscallHandler : public InterruptHandler {
 public:
  SyscallHandler(InterruptManager* manager);
  ~SyscallHandler();
  virtual uint32_t operator()(uint32_t esp);
};

#endif
