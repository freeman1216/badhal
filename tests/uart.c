#define BAD_PLLM (25)
#define BAD_PLLN (400)
#define BAD_PLLQ (10)
#define BAD_PLLP (PLLP4)

#define BAD_AHB_PRE     (HPRE_DIV_1)
#define BAD_APB1_PRE    (PPRE_DIV_2)
#define BAD_APB2_PRE    (PPRE_DIV_1)

#define BAD_RCC_IMPLEMENTATION
#define BAD_GPIO_IMPLEMENTATION
#define BAD_TIMER_IMPLEMENTATION
#define BAD_USART_IMPLEMENTATION
#define BAD_FLASH_IMPLEMENTATION
#define BAD_SYSTICK_IMPLEMETATION

#define BAD_SYSTICK_SYSTICK_ISR_IMPLEMENTATION
#define BAD_HARDFAULT_IMPLEMENTATION
#define BAD_HARDFAULT_USE_UART
#include "badhal.h"

#define UART_GPIO_PORT          (GPIOA)
#define UART1_TX_PIN            (9)
#define UART1_RX_PIN            (10)
#define UART1_TX_AF             (7)
#define UART1_RX_AF             (7)
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

#define BAD_UART_TEST_AHB1_PERIPEHRALS      (RCC_AHB1_GPIOA)
#define BAD_UART_TEST_APB2_PERIPHERALS      (RCC_APB2_USART1)
#define BAD_UART_TEST_SETTINGS              (USART_FEATURE_TRANSMIT_EN|USART_FEATURE_RECIEVE_EN)

static inline void __main_clock_setup(){
    rcc_enable_hse();
    rcc_pll_setup( PLLP4, BADHAL_PLLM, BADHAL_PLLN, BADHAL_PLLQ, PLL_SOURCE_HSE);
    rcc_bus_prescalers_setup(HPRE_DIV_1, PPRE_DIV_2, PPRE_DIV_1);
    flash_acceleration_setup(BADHAL_FLASH_LATENCY, FLASH_DCACHE_ENABLE, FLASH_ICACHE_ENABLE);
    rcc_enable_and_switch_to_pll();
}

static inline void __periph_setup(){
    rcc_set_ahb1_clocking(BAD_UART_TEST_AHB1_PERIPEHRALS);
    io_setup_pin(UART_GPIO_PORT, UART1_TX_PIN, MODER_af, UART1_TX_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(UART_GPIO_PORT, UART1_RX_PIN, MODER_af, UART1_RX_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    //Enable UART clocking
    rcc_set_apb2_clocking(BAD_UART_TEST_APB2_PERIPHERALS);
}

static inline void __uart_setup(){
    uart_setup(USART1, USART_BRR_115200, BAD_UART_TEST_SETTINGS, 0, 0);
    uart_enable(USART1);
}

static inline void __systick_setup(){
    systick_setup(CLOCK_SPEED/1000, SYSTICK_FEATURE_CLOCK_SOURCE|SYSTICK_FEATURE_TICK_INTERRUPT);
    systick_enable();
}

volatile uint32_t ticks;

void systick_usr(){
    ++ticks;
}

int main(){
    __DISABLE_INTERUPTS;
    __main_clock_setup();
    __periph_setup();
    __uart_setup();   
    __systick_setup();
    
    __ENABLE_INTERUPTS;
    uint32_t now = 0;
    uint32_t prev =0;
    uint32_t interval= 500;
    while(1){
        now = ticks;
        if(now - prev >= interval){
            uart_send_dec_unsigned_32bit(USART1, now);
            prev = now;
        }
    }
    return 0;
}
