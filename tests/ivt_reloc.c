#define BAD_RCC_IMPLEMENTATION
#define BAD_GPIO_IMPLEMENTATION
#define BAD_TIMER_IMPLEMENTATION
#define BAD_USART_IMPLEMENTATION
#define BAD_FLASH_IMPLEMENTATION

#define BTIMER_TIM1_UP_TIM10_ISR_IMPLEMENTATION
#define BTIMER_USE_TIM10_USR
#define BAD_HARDFAULT_IMPLEMENTATION
#define BAD_HARDFAULT_USE_UART
#include "badhal.h"

#define UART_GPIO_PORT          (GPIOA)
#define UART1_TX_PIN            (9)
#define UART1_RX_PIN            (10)
#define UART1_TX_AF             (7)
#define UART1_RX_AF             (7)
// Exti pin




    // HSE  = 25
    // PLLM = 25
    // PLLN = 400
    // PLLQ = 10
    // PLLP = 4
    // Sysclock = 100
#define BADHAL_PLLM (25)
#define BADHAL_PLLN (400)
#define BADHAL_PLLQ (10)
#define BADHAL_FLASH_LATENCY (FLASH_LATENCY_3ws)

#define BAD_BTIMER_TEST_AHB1_PERIPEHRALS    (RCC_AHB1_GPIOA)
#define BAD_BTIMER_TEST_APB2_PERIPHERALS    (RCC_APB2_USART1|RCC_APB2_TIM10)

#define BAD_BTIMER_TEST_ARR    (65535)
#define BAD_BTIMER_TEST_PSC    (1525)
#define BAD_BTIMER_TEST_INTR   (BTIMER_UPDATE)

extern uint32_t __ram_ivt;

uint32_t secondselapsed;

void ATTR_RAMFUNC tim10_usr(){
    secondselapsed++;
}


static inline void __main_clock_setup(){
    rcc_enable_hse();
    rcc_pll_setup( PLLP4, BADHAL_PLLM, BADHAL_PLLN, BADHAL_PLLQ, PLL_SOURCE_HSE);
    rcc_bus_prescalers_setup(HPRE_DIV_1, PPRE_DIV_2, PPRE_DIV_1);
    flash_acceleration_setup(BADHAL_FLASH_LATENCY, FLASH_DCACHE_ENABLE, FLASH_ICACHE_ENABLE);
    rcc_enable_and_switch_to_pll();
}

static inline void __periph_setup(){
    rcc_set_ahb1_clocking(BAD_BTIMER_TEST_AHB1_PERIPEHRALS);
    io_setup_pin(UART_GPIO_PORT, UART1_TX_PIN, MODER_af, UART1_TX_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(UART_GPIO_PORT, UART1_RX_PIN, MODER_af, UART1_RX_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    //Enable UART clocking
    rcc_set_apb2_clocking(BAD_BTIMER_TEST_APB2_PERIPHERALS);
}


static inline void __timer_setup(){
    basic_timer_setup(BTIM10, BAD_BTIMER_TEST_ARR, BAD_BTIMER_TEST_PSC, BAD_BTIMER_TEST_INTR);
    nvic_enable_interrupt(NVIC_TIM1_UP_TIM10_INTR);
    tim_enable(BTIM10);
}


int main(){
    __DISABLE_INTERUPTS;
    __main_clock_setup();
    __periph_setup();
    SCB->VTOR = (uint32_t)&__ram_ivt;
    __timer_setup(); 
   

    
    __ENABLE_INTERUPTS;

    while(1){

    }
    return 0;
}
