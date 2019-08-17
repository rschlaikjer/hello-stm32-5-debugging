#pragma once

#include <cstdint>

#include <arm/base.hpp>

namespace arm {
namespace mpu {

//// ARM MPU register locations
// Type Register (RO)
static const Reg32 &MPU_TYPE = Register<uint32_t>(0xE000ED90);
// Control Register (RW)
static Reg32 &MPU_CTRL = Register<uint32_t>(0xE000ED94);
// Region Number Register (RW)
static Reg32 &MPU_RNR = Register<uint32_t>(0xE000ED98);
// Region Base Address Register (RW)
static Reg32 &MPU_RBAR = Register<uint32_t>(0xE000ED9C);
// Region Attribute and Size Register (RW)
static Reg32 &MPU_RASR = Register<uint32_t>(0xE000EDA0);

//// Register subfields
// Type register
const uint32_t MPU_TYPE_DREGION_MASK = 0x0000FF00;
const uint32_t MPU_TYPE_DREGION_SHIFT = 8;
// Control register
const uint32_t MPU_CTRL_ENABLE = (1 << 0);
const uint32_t MPU_CTRL_HFNMIENA = (1 << 1);
const uint32_t MPU_CTRL_PRIVDEFENA = (1 << 2);
// Region number register
const uint32_t MPU_RNR_REGION_MASK = 0x000000FF;
const uint32_t MPU_RNR_REGION_SHIFT = 0;
// Region Base Address Register
const uint32_t MPU_RBAR_ADDR_MASK = 0xFFFFFFE0;
const uint32_t MPU_RBAR_VALID = (1 << 4);
const uint32_t MPU_RBAR_REGION_MASK = 0x0000000F;
// Region Attribute and Size Register
const uint32_t MPU_RASR_ENABLE = (1 << 0);
const uint32_t MPU_RASR_SIZE_MASK = 0x0000001E;
const uint32_t MPU_RASR_SIZE_SHIFT = 1;
const uint32_t MPU_RASR_SRD_MASK = 0x0000FF00;
const uint32_t MPU_RASR_SRD_SHIFT = 8;
const uint32_t MPU_RASR_B = (1 << 16);
const uint32_t MPU_RASR_C = (1 << 17);
const uint32_t MPU_RASR_S = (1 << 18);
const uint32_t MPU_RASR_TEX_MASK = 0x00380000;
const uint32_t MPU_RASR_TEX_SHIFT = 19;
const uint32_t MPU_RASR_AP_MASK = 0x07000000;
const uint32_t MPU_RASR_AP_SHIFT = 24;
const uint32_t MPU_RASR_XN = (1 << 28);

enum class RegionSize : uint32_t {
  SZ_32B = 4,
  SZ_64B = 5,
  SZ_128B = 6,
  SZ_256B = 7,
  SZ_512B = 8,
  SZ_1K = 9,
  SZ_2K = 10,
  SZ_4K = 11,
  SZ_8K = 12,
  SZ_16K = 13,
  SZ_32K = 14,
  SZ_64K = 15,
};

enum class AccessPermission : uint32_t {
  // Any access generates a permission fault
  NO_ACCESS = 0b000,
  // Privileged access only
  PRIV_RW = 0b001,
  // Any unprivileged write generates an access fault
  PRIV_RW_UNPRIV_RO = 0b010,
  // Full access
  ALL_RW = 0b011,
  // Privileged read-only
  PRIV_RO = 0b101,
  // Privileged and unprivileged read-only
  PRIV_RO_UNPRIV_RO = 0b110,
};

// Cacheable / Bufferable / Shareable profiles.
// ARM recommends that for a microcontroller, one use the following:
// Memory        |  C  |  B  |  S  |
// --------------+-----+-----+-----+
// Flash         |  1  |  0  |  0  |
// Internal SRAM |  1  |  0  |  1  |
// External SRAM |  1  |  1  |  1  |
// Peripherals   |  0  |  1  |  1  |
enum class CBSProfile : uint32_t {
  FLASH = (MPU_RASR_C),
  INTERNAL_SRAM = (MPU_RASR_C | MPU_RASR_S),
  EXTERNAL_SRAM = (MPU_RASR_C | MPU_RASR_B | MPU_RASR_S),
  PERIPHERALS = (MPU_RASR_B | MPU_RASR_S),
};

uint8_t supported_region_count();
void set_mpu_enabled(bool enable);
void enable_default_map_in_priveliged_mode(bool enable);
void set_active_region(uint8_t region);
void set_active_region_base_address(void *base_addr);
void set_region_base_address(uint8_t region, void *base_addr);
void set_active_region_enabled(bool enabled);
void set_active_region_size(RegionSize size);
void set_active_region_access_permission(AccessPermission ap);
void set_active_region_execute_never(bool xn);
void set_active_region_cbs(bool cacheable, bool bufferable, bool shareable);
void set_active_region_type(CBSProfile prof);

// Convenience method for setting up an entire region at once
void configure_region(uint8_t region, void *base_address, RegionSize size,
                      AccessPermission permissions, bool execute_never,
                      CBSProfile profile);

} // namespace mpu
} // namespace arm

