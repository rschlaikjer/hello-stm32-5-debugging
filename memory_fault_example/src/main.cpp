#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>

#include <arm/mpu.hpp>
#include <arm/scb.hpp>

// Constants for our timer setup
static const rcc_periph_clken update_timer_rcc = RCC_TIM3;
static const uint32_t update_timer_nvic = NVIC_TIM3_IRQ;
static const uint32_t update_timer = TIM3;

int main(void) {
  // Set clock at 216MHz from internal oscillator
  rcc_clock_setup_hse(&rcc_3v3[RCC_CLOCK_3V3_216MHZ], 8);

  // Create a protected memory region at address 0x0
  // Allow privileged mode code to see the default map
  arm::mpu::enable_default_map_in_priveliged_mode(true);
  // Enable MPU
  arm::mpu::set_mpu_enabled(true);

  //// Enable MPU protections for the ITCM code under region 0
  arm::mpu::configure_region(0, 0x0, arm::mpu::RegionSize::SZ_16K,
                             arm::mpu::AccessPermission::PRIV_RO_UNPRIV_RO,
                             false, arm::mpu::CBSProfile::INTERNAL_SRAM);
  // Enable region
  arm::mpu::set_active_region_enabled(true);
  // Enable mem fault handler
  arm::scb::set_mem_fault_handler_enabled(true);

  //// Set up our debug UART for fault messages
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_USART1);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO10);
  gpio_set_af(GPIOA, GPIO_AF1, GPIO9 | GPIO10);
  usart_set_baudrate(USART1, 115200);
  usart_set_databits(USART1, 8);
  usart_set_parity(USART1, USART_PARITY_NONE);
  usart_set_stopbits(USART1, USART_CR2_STOPBITS_1);
  usart_set_mode(USART1, USART_MODE_TX_RX);
  usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
  usart_enable(USART1);

  //// Set up a timer to generate an interrupt we won't handle
  // Enable clock to the timer
  rcc_periph_clock_enable(update_timer_rcc);
  // Enable interrupts for the timer
  nvic_enable_irq(update_timer_nvic);
  // Init the timer to count up
  timer_set_mode(update_timer, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1,
                 TIM_CR1_DIR_UP);
  // Set continuous mode
  timer_continuous_mode(update_timer);
  // Prescale to millisecond range
  timer_set_prescaler(update_timer, rcc_apb2_frequency / 1000);
  // Set period to be three seconds
  timer_set_period(update_timer, 3000);
  // Enable timer update IRQ
  timer_enable_irq(update_timer, TIM_DIER_UIE);
  // Start it
  timer_set_counter(update_timer, 0);
  timer_enable_counter(update_timer);

  // Now we just wait and hope nothing goes wrong...
  while (true) {
  }
}

void tim3_isr(void) {
  // Perform a nasty memory access...
  uint32_t *ptr = reinterpret_cast<uint32_t *>(0x0);
  *ptr = 1234;
}
