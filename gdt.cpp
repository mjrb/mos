#include "gdt.h"

GlobalDescriptorTable::GlobalDescriptorTable()
  : null(0,0,0), unused(0,0,0),
    code(0, 64 * 1024 * 1024, 0x9A), data(0, 64 * 1024 * 1024, 0x92) {
  uint32_t i[2];
  i[1] = (uint32_t)this;
  i[0] = sizeof(GlobalDescriptorTable) << 16;
  
  asm volatile("lgdt (%0)": :"p"(((uint8_t*) i) + 2));
}

GlobalDescriptorTable::~GlobalDescriptorTable() {
  
}

uint16_t GlobalDescriptorTable::code_offset() {
  return (uint8_t*)&code - (uint8_t*)this;
}
uint16_t GlobalDescriptorTable::data_offset() {
  return (uint8_t*)&data - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base,
							    uint32_t bounds,
							    uint8_t flags) {
  uint8_t* target = (uint8_t*)this;
  if (bounds <= 0x10000) {
    target[6] = 0x40;
  } else {
    if ((bounds & 0xFFF) != 0xFFF) {
      bounds = (bounds >> 12) - 1;
    } else {
      bounds = (bounds >> 12);
    }
    target[6] = 0xC0;
  }

  // set bounds
  target[0] = bounds & 0xFF;
  target[1] = (bounds >> 8) & 0xFF;
  target[6] |= (bounds >> 16) & 0xF;

  // set base
  target[2] = base & 0xFF;
  target[3] = (base >> 8) & 0xFF;
  target[4] = (base >> 16) & 0xFF;
  target[7] = (base >> 24) & 0xFF;

  // set flags
  target[5] = flags;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::base() {
  uint8_t* source = (uint8_t*)this;
  uint32_t result = source[7];
  result = (result << 8) | source[4];
  result = (result << 8) | source[3];
  result = (result << 8) | source[2];
  return result;
}
uint32_t GlobalDescriptorTable::SegmentDescriptor::bounds() {
  uint8_t* source = (uint8_t*)this;
  uint32_t result = source[6] & 0xF;
  result = (result << 4) | source[1];
  result = (result << 8) | source[0];
  if ((source[6] & 0xC0) == 0xC0) {
    result = (result << 12) | 0xFFF;
  }
  return result;
}
