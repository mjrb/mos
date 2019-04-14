#ifndef GDT_H
#define GDT_H
#include "types.h"

class GlobalDescriptorTable {
public:
  class SegmentDescriptor {
  private:
    uint16_t limit_lo;
    uint16_t base_lo;
    uint8_t base_hi;
    uint8_t type;
    uint8_t flags_limit_hi;
    uint8_t base_vhi;
    
  public:
    SegmentDescriptor(uint32_t base, uint32_t bounds, uint8_t type);
    uint32_t base();
    uint32_t bounds();
  } __attribute__((packed));
  
private:
  SegmentDescriptor null;
  SegmentDescriptor unused;
  SegmentDescriptor code;
  SegmentDescriptor data;
  
public:
  GlobalDescriptorTable();
  ~GlobalDescriptorTable();
  uint16_t code_offset();
  uint16_t data_offset();
};
#endif
