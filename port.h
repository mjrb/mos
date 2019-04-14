#ifndef PORT_H
#define PORT_H
#include "types.h"

template<typename T>
class Port {
protected:
  uint16_t port;
public:
  Port(uint16_t port);
  ~Port();
  virtual void write(T data);
  virtual T read();
};

using Port8 = Port<uint8_t>;
using Port16 = Port<uint16_t>;
using Port32 = Port<uint32_t>;

class Port8slow : public Port<uint8_t> {
public:
  Port8slow(uint16_t port);
  ~Port8slow();
  virtual void write(uint8_t data);
};

#endif
