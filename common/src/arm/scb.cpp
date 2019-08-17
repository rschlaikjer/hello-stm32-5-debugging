#include <arm/scb.hpp>

namespace arm {
namespace scb {

void set_mem_fault_handler_enabled(bool enable) {
  // Enable / disable mem fault handler
  if (enable) {
    SHCSR |= SHCSR_MEMFAULTENA;
  } else {
    SHCSR &= ~SHCSR_MEMFAULTENA;
  }
}

} // namespace scb
} // namespace arm

