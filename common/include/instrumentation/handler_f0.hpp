#pragma once

#if not defined(STM32F0)
#error "Atempting to import STM32F0 fault handlers, but STM family not defined!"
#endif

void wwdg_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void pvd_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void rtc_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void flash_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void rcc_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void exti0_1_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void exti2_3_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void exti4_15_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void tsc_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void dma1_channel1_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void dma1_channel2_3_dma2_channel1_2_isr(void)
    __attribute__((weak, alias("hard_fault_handler")));
void dma1_channel4_7_dma2_channel3_5_isr(void)
    __attribute__((weak, alias("hard_fault_handler")));
void adc_comp_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void tim1_brk_up_trg_com_isr(void)
    __attribute__((weak, alias("hard_fault_handler")));
void tim1_cc_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void tim2_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void tim3_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void tim6_dac_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void tim7_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void tim14_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void tim15_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void tim16_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void tim17_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void i2c1_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void i2c2_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void spi1_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void spi2_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void usart1_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void usart2_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void usart3_4_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void cec_can_isr(void) __attribute__((weak, alias("hard_fault_handler")));
void usb_isr(void) __attribute__((weak, alias("hard_fault_handler")));
