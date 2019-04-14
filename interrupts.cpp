#include "interrupts.h"

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];
InterruptManager* InterruptManager::current = 0;

uint32_t InterruptManager::handleInterrupt(uint8_t num, uint32_t esp) {
  if (current != 0) {
    current->instHandleInterrupt(num, esp);
  }
  return esp;
}
uint32_t InterruptManager::instHandleInterrupt(uint8_t num, uint32_t esp) {
  printf("INTERRUPT ");
  printh(num);
  printf("! ");


  // if its a hardware interrupt 0x20-0x30, then acknowlage the pic
  if (0x20 <= num && num <= 0x30) {
    pic_master_command.write(0x20);
    if (0x28 <= num) {
      pic_slave_command.write(0x20);
    }
  }
  
  return esp;
}


void InterruptManager::setInterruptDescriptorTableEntry(uint8_t intnum,
							uint16_t gdt_code_segment,
							uint8_t privilege_level,
							uint8_t type,
							void (*handler)()) {
  InterruptManager::GateDescriptor* ent = &interruptDescriptorTable[intnum];

  const uint8_t IDT_DESC_PRESENT = 0x80;
  
  ent->handler_low = ((uint32_t)handler) & 0xFFFF;
  ent->handler_high = ((uint32_t)handler >> 16) & 0xFFFF;
  ent->gdt_code_segment = gdt_code_segment;
  ent->access = IDT_DESC_PRESENT | ((privilege_level & 3) << 5) | type;
  ent->reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) :
  pic_master_command(0x20), pic_master_data(0x21),
  pic_slave_command(0xA0), pic_slave_data(0xA1)
{
  uint16_t code = gdt->code_offset();
  const uint8_t IDT_INTERRUPT_GATE = 0xE;
  for (uint16_t i = 0; i < 256; i++) {
    setInterruptDescriptorTableEntry(i, code, 0, IDT_INTERRUPT_GATE,
				     &ignoreInterrupt);
  }
  setInterruptDescriptorTableEntry(0x20, code, 0, IDT_INTERRUPT_GATE,
				   &handleInterruptRequest0x00);
  setInterruptDescriptorTableEntry(0x21, code, 0, IDT_INTERRUPT_GATE,
				   &handleInterruptRequest0x01);

  // begin sending interupts
  pic_master_command.write(0x11);
  pic_slave_command.write(0x11);

  // offset so we don't reuse interupts 0-20
  // these interupts are reserved by cpu for exceptions
  pic_master_data.write(0x20);
  pic_slave_data.write(0x28);

  // tell slave and master who is who
  pic_master_data.write(0x04);
  pic_slave_data.write(0x02);

  // ??
  pic_master_data.write(0x01);
  pic_slave_data.write(0x01);

  pic_master_data.write(0x00);
  pic_slave_data.write(0x00);

  InterruptDescriptorTable idt;
  idt.size = 256 * sizeof(GateDescriptor) - 1;
  idt.base = (uint32_t)interruptDescriptorTable;
  asm volatile("lidt %0" : : "m"(idt));
  
}
InterruptManager::~InterruptManager() {
  
}
void InterruptManager::activate() {
  if (current != 0) {
    current->deactivate();
  }
  current = this;
  asm("sti");
}
void InterruptManager::deactivate() {
  if (current = this) {
    asm("cli");
    current = 0;
  }
}

