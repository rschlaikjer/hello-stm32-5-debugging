#include <arm/mpu.hpp>

namespace arm {
namespace mpu {

uint8_t supported_region_count() {
  // Get the number of regions supported by the MPU.
  // This field will be zero if the processor does not implement an MPU.
  return (MPU_TYPE & MPU_TYPE_DREGION_MASK) >> MPU_TYPE_DREGION_SHIFT;
}

void set_mpu_enabled(bool enable) {
  // Enable / disable the MPU
  if (enable) {
    MPU_CTRL |= MPU_CTRL_ENABLE;
  } else {
    MPU_CTRL &= ~MPU_CTRL_ENABLE;
  }
}

void enable_default_map_in_priveliged_mode(bool enable) {
  // If this bit is cleared, then any instruction that does not access a
  // defined region will fault.
  // If set, then the default memory map is enabled as a background region for
  // privileged access. Regions explicitly configured in the MPU override the
  // background map.
  if (enable) {
    MPU_CTRL |= MPU_CTRL_PRIVDEFENA;
  } else {
    MPU_CTRL &= ~MPU_CTRL_PRIVDEFENA;
  }
}

void set_active_region(uint8_t region) {
  // The MPU_RBAR and MPU_RASR registers operate on the region currently
  // selected in the MPU_RNR register.
  // This method updates the selected region.
  MPU_RNR =
      (MPU_RNR & (~MPU_RNR_REGION_MASK)) | (region << MPU_RNR_REGION_SHIFT);
}

void set_active_region_base_address(void *base_addr) {
  // Set the base address for the currently active MPU region.
  MPU_RBAR = (reinterpret_cast<uint32_t>(base_addr) & MPU_RBAR_ADDR_MASK);
}

void set_region_base_address(uint8_t region, void *base_addr) {
  // Set the base address for a specific region. This can be done in a single
  // operation using the combination of the VALID bit and the low 4 bits to
  // select a region.
  MPU_RBAR = ((reinterpret_cast<uint32_t>(base_addr) & MPU_RBAR_ADDR_MASK) |
              MPU_RBAR_VALID | (MPU_RBAR_REGION_MASK & region));
}

void set_active_region_enabled(bool enabled) {
  // Enable or disable the currently selected region
  if (enabled) {
    MPU_RASR |= MPU_RASR_ENABLE;
  } else {
    MPU_RASR &= ~MPU_RASR_ENABLE;
  }
}

void set_active_region_size(RegionSize size) {
  // Set the size of the currently selected region.
  // The size field is 4 bits, and the size is configured as 2^(size + 1)
  // Size values < 4 are not defined, so the smallest possible region is 32
  // bytes.
  MPU_RASR = (MPU_RASR & ~MPU_RASR_SIZE_MASK) |
             (static_cast<uint32_t>(size) << MPU_RASR_SIZE_SHIFT);
}

void set_active_region_access_permission(AccessPermission ap) {
  // Set the access permissions of the current region.
  // See the AccessPermission enum class for possible configurations.
  MPU_RASR = (MPU_RASR & ~MPU_RASR_AP_MASK) |
             (static_cast<uint32_t>(ap) << MPU_RASR_AP_SHIFT);
}

void set_active_region_execute_never(bool xn) {
  // If the XN bit is set, any attempt to load an instruction from the current
  // region will trigger a fault.
  if (xn) {
    MPU_RASR |= MPU_RASR_XN;
  } else {
    MPU_RASR &= ~MPU_RASR_XN;
  }
}

void set_active_region_cbs(bool cacheable, bool bufferable, bool shareable) {
  // Configure the cacheable, bufferable and shareable setting for a region.
  const uint32_t mask =
      ((cacheable ? MPU_RASR_C : 0x0) | (bufferable ? MPU_RASR_B : 0x0) |
       (shareable ? MPU_RASR_S : 0x0));
  MPU_RASR = (MPU_RASR & ~(MPU_RASR_C | MPU_RASR_B | MPU_RASR_S)) | (mask);
}

void set_active_region_type(CBSProfile prof) {
  // Configure the C/B/S flags using a named profile.
  MPU_RASR = (MPU_RASR & ~(MPU_RASR_C | MPU_RASR_B | MPU_RASR_S)) |
             (static_cast<uint32_t>(prof));
}
void configure_region(uint8_t region, void *base_address, RegionSize size,
                      AccessPermission permissions, bool execute_never,
                      CBSProfile profile) {
  // Set the active region and base address in one go
  set_region_base_address(region, base_address);
  // Set region size
  set_active_region_size(size);
  // Access perms
  set_active_region_access_permission(permissions);
  // Can we execute from this region
  set_active_region_execute_never(execute_never);
  // Apply memory profile
  set_active_region_type(profile);
}

} // namespace mpu
} // namespace arm

