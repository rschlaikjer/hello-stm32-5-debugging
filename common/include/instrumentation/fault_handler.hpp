#ifndef _MT_CORE__INSTRUMENTATION_FAULT_HANDLER_H
#define _MT_CORE__INSTRUMENTATION_FAULT_HANDLER_H

#include <libopencm3/cm3/nvic.h>

// ARM Interupt Control and State Register
#define NVIC_ICSR MMIO32(0xE000ED04)

extern "C" {
void base_fault_handler(uint32_t stack[]);
void dump_stack(uint32_t stack[]);
}

#if defined(STM32F0)
#include <instrumentation/handler_f0.hpp>
#elif defined(STM32F7)
#include <instrumentation/handler_f7.hpp>
#include <libopencm3/cm3/itm.h>
#else
#error "Unable to install fault handler for unknown STM32 family!"
#endif

#endif // _MT_CORE__INSTRUMENTATION_FAULT_HANDLER_H
