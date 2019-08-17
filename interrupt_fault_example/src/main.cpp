#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>

// Constants for our timer setup
static const rcc_periph_clken update_timer_rcc = RCC_TIM3;
static const uint32_t update_timer_nvic = NVIC_TIM3_IRQ;
static const uint32_t update_timer = TIM3;

int main(void) {
  // Set clock at 48MHz from internal oscillator
  rcc_clock_setup_in_hsi_out_48mhz();

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
