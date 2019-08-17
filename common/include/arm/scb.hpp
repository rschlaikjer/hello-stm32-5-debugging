#pragma once

#include <cstdint>

#include <arm/base.hpp>

namespace arm {
namespace scb {

//// ARM SCB register locations
// CPUID register (RO)
static const Reg32 &CPUID = Register<uint32_t>(0xE000ED00);
// Interrupt control and state register (RW)
static Reg32 &ICSR = Register<uint32_t>(0xE000ED04);
// Vector Table Offset Register (RW)
static Reg32 &VTOR = Register<uint32_t>(0xE000ED08);
// Configuration and Control register
static Reg32 &CCR = Register<uint32_t>(0xE000ED14);
// System Handler Control and State Register
static Reg32 &SHCSR = Register<uint32_t>(0xE000ED24);
// Configurable Fault Status Register
static Reg32 &CFSR = Register<uint32_t>(0xE000ED28);
// Hard Fault Status Register (RW)
static Reg32 &HFSR = Register<uint32_t>(0xE000ED2C);
// Debug Fault Status Register
static Reg32 &DFSR = Register<uint32_t>(0xE000ED30);
// MemManage Fault Address Register
static Reg32 &MMFAR = Register<uint32_t>(0xE000ED34);
// BusFault Address Register
static Reg32 &BFAR = Register<uint32_t>(0xE000ED38);

//// Register subfields
// CFSR
// 1 if MMFAR has valid contents
const uint32_t CFSR_MMFSR_MMARVALID = (1 << 7);
// 1 if fault occurred during FP lazy state preservation
const uint32_t CFSR_MMFSR_MLSPERR = (1 << 5);
// 1 if fault occurred on exception entry
const uint32_t CFSR_MMFSR_MSTKERR = (1 << 4);
// 1 if fauly occurred on exception return
const uint32_t CFSR_MMFSR_MUNSTKERR = (1 << 3);
// 1 if a data access violation occurred
const uint32_t CFSR_MMFSR_DACCVIOL = (1 << 1);
// 1 if an eXecute Never violation has occurred
const uint32_t CFSR_MMFSR_IACCVIOL = (1 << 0);

// SHCSR
const uint32_t SHCSR_MEMFAULTENA = (1 << 16);

// Methods
void set_mem_fault_handler_enabled(bool enable);

} // namespace scb
} // namespace arm

