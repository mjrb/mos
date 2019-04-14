#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "types.h"
#include "print.h"
#include "gdt.h"
#include "port.h"

class InterruptManager;

class InterruptHandler {
protected:
  uint8_t num;
  InterruptManager* manager;
  InterruptHandler(int num, InterruptManager* manager);
  ~InterruptHandler();
public:
  virtual uint32_t operator()(uint32_t esp);
};

class InterruptManager {
  friend class InterruptHandler;
protected:
  InterruptHandler* handlers[256];
  
  static InterruptManager* current;
  
  struct GateDescriptor {
    uint16_t handler_low;
    uint16_t gdt_code_segment;
    uint8_t reserved;
    uint8_t access;
    uint16_t handler_high;
    
  } __attribute__((packed));
  static GateDescriptor interruptDescriptorTable[256];
  static void setInterruptDescriptorTableEntry(uint8_t intnum,
					      uint16_t gdt_code_segment,
					      uint8_t privilege_level,
					      uint8_t type,
					      void (*handler)());
  struct InterruptDescriptorTable {
    uint16_t size;
    uint32_t base;
  } __attribute__((packed));

  Port8slow pic_master_command;
  Port8slow pic_master_data;
  Port8slow pic_slave_command;
  Port8slow pic_slave_data;

public:
  InterruptManager(GlobalDescriptorTable* gdt);
  ~InterruptManager();
  
  void activate();
  void deactivate();

  // calls instHandleInterrupt on the current instance
  static uint32_t handleInterrupt(uint8_t num, uint32_t esp);
  uint32_t instHandleInterrupt(uint8_t num, uint32_t esp);
  
  static void ignoreInterrupt();
  
  static void handleInterruptRequest0x00();
  static void handleInterruptRequest0x01();
  static void handleException0x01();
};

#endif
