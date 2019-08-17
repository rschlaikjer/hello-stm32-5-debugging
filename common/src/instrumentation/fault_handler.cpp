#include <arm/scb.hpp>
#include <instrumentation/fault_handler.hpp>
#include <libopencm3/stm32/usart.h>

// Create weak sprintf symbol for targets that do not include printf
extern "C" {
int dummy_sprintf(__attribute__((unused)) char *str,
                  __attribute__((unused)) const char *format, ...) {
  // Nop
  return 0;
}
int sprintf_(char *str, const char *format, ...)
    __attribute__((weak, alias("dummy_sprintf")));
}

void fault_handler_printf(const char *msg) {
#ifdef ITM_BASE
  // On a MCU with ITM support, log to ITM port 0
  while (*msg) {
    ITM_STIM8(0) = *msg;
    msg++;
  }
#else
  // For the purposes of this example, if there's no ITM just attempt to
  // send debug info on USART
  while (*msg) {
    usart_send_blocking(USART1, *msg);
    msg++;
  }
#endif
}

enum { r0, r1, r2, r3, r12, lr, pc, psr };

// Names for exception number definitions, table B1-4, ARMv7-M Architecture
// Reference Manual
static const char *system_interrupt_names[16] = {
    "SP_Main",      "Reset",    "NMI",        "Hard Fault",
    "MemManage",    "BusFault", "UsageFault", "Reserved",
    "Reserved",     "Reserved", "Reserved",   "SVCall",
    "DebugMonitor", "Reserved", "PendSV",     "SysTick"};

void dump_stack(uint32_t stack[]) {
  static char msg[80];
  sprintf_(msg, "r0  = 0x%08x\r\n", stack[r0]);
  fault_handler_printf(msg);
  sprintf_(msg, "r1  = 0x%08x\r\n", stack[r1]);
  fault_handler_printf(msg);
  sprintf_(msg, "r2  = 0x%08x\r\n", stack[r2]);
  fault_handler_printf(msg);
  sprintf_(msg, "r3  = 0x%08x\r\n", stack[r3]);
  fault_handler_printf(msg);
  sprintf_(msg, "r12 = 0x%08x\r\n", stack[r12]);
  fault_handler_printf(msg);
  sprintf_(msg, "lr  = 0x%08x\r\n", stack[lr]);
  fault_handler_printf(msg);
  sprintf_(msg, "pc  = 0x%08x\r\n", stack[pc]);
  fault_handler_printf(msg);
  sprintf_(msg, "psr = 0x%08x\r\n", stack[psr]);
  fault_handler_printf(msg);
}

void base_fault_handler(uint32_t stack[]) {
  fault_handler_printf("\r\n\r\nFault encountered!\r\n");
  static char buf[64];
  // Get the fault cause. Volatile to prevent compiler elision.
  const volatile uint8_t active_interrupt = NVIC_ICSR & 0xFF;
  if (active_interrupt < 16) {
    sprintf_(buf, "Cause: %s (%u)\r\n",
             system_interrupt_names[active_interrupt], active_interrupt);
  } else {
    // External (user) interrupt
    sprintf_(buf, "Unimplemented user interrupt %u\r\n", active_interrupt - 16);
  }
  fault_handler_printf(buf);

  fault_handler_printf("Stacktrace:\r\n");
  dump_stack(stack);
  __asm volatile("BKPT #01");
  while (1) {
  }
}

void hard_fault_handler(void) {
  __asm("MRS r0, MSP\r\n" // Default to the Main Stack Pointer
        "MOV r1, lr\r\n"  // Load the current link register value
        "MOVS r2, #4\r\n" // Load constant 4
        "TST r1, r2\r\n"  // Test whether we are in master or thread mode
        "BEQ base_fault_handler\r\n" // If in master mode, MSP is correct.
        "MRS r0, PSP\r\n" // If we weren't in master mode, load PSP instead
        "B base_fault_handler"); // Jump to the fault handler.
}

void mem_manage_handler(void) {
  // Pull the MMFSR data out into variables for easy inspection
  const volatile bool mmfar_valid =
      arm::scb::CFSR & arm::scb::CFSR_MMFSR_MMARVALID;
  const volatile bool fp_lazy_error =
      arm::scb::CFSR & arm::scb::CFSR_MMFSR_MLSPERR;
  const volatile bool exception_entry_error =
      arm::scb::CFSR & arm::scb::CFSR_MMFSR_MSTKERR;
  const volatile bool exception_exit_error =
      arm::scb::CFSR & arm::scb::CFSR_MMFSR_MUNSTKERR;
  const volatile bool data_access_error =
      arm::scb::CFSR & arm::scb::CFSR_MMFSR_DACCVIOL;
  const volatile bool instruction_access_error =
      arm::scb::CFSR & arm::scb::CFSR_MMFSR_IACCVIOL;

  // Pull the MMFAR address
  const volatile uint32_t mmfar_address = arm::scb::MMFAR;

  // Trigger a breakpoint
  __asm volatile("BKPT #01");
}

// Alias all other fault handlers to the hardfault handler
void bus_fault_handler(void) __attribute__((alias("hard_fault_handler")));
void usage_fault_handler(void) __attribute__((alias("hard_fault_handler")));
