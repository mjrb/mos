#include "port.h"

template<typename T>
Port<T>::Port(uint16_t port) : port(port) {};

template<typename T>
Port<T>::~Port(){};

template<>
void Port<uint8_t>::write(uint8_t data) {
  __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

template<>
void Port<uint16_t>::write(uint16_t data) {
  __asm__ volatile("outw %0, %1" : : "a"(data), "Nd"(port));
}

template<>
void Port<uint32_t>::write(uint32_t data) {
  __asm__ volatile("outl %0, %1" : : "a"(data), "Nd"(port));
}

template<>
uint8_t Port<uint8_t>::read() {
  uint8_t result;
  __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
  return result;
}

template<>
uint16_t Port<uint16_t>::read() {
  uint16_t result;
  __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
  return result;
}

template<>
uint32_t Port<uint32_t>::read() {
  uint32_t result;
  __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(port));
  return result;
}

Port8slow::Port8slow(uint16_t port) : Port8(port) {};
Port8slow::~Port8slow() {
}

void Port8slow::write(uint8_t data) {
  __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a"(data), "Nd"(port));
}
