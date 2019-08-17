#pragma once

namespace arm {

// Convenience template for taking an integer register address and converting
// to a reference to that address.
template <typename T> constexpr T &Register(uint32_t addr) {
  return *reinterpret_cast<T *>(addr);
}

// Typedefs for register references of 32, 16 and 8 bits.
using Reg32 = volatile uint32_t;
using Reg16 = volatile uint16_t;
using Reg8 = volatile uint8_t;

} // namespace arm
