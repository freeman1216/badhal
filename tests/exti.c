#define BAD_PLLM (25)
#define BAD_PLLN (400)
#define BAD_PLLQ (10)

#define BAD_AHB_PRE     (HPRE_DIV_1)
#define BAD_APB1_PRE    (PPRE_DIV_2)
#define BAD_APB2_PRE    (PPRE_DIV_1)

#define BAD_RCC_IMPLEMENTATION
#define BAD_GPIO_IMPLEMENTATION
#define BAD_EXTI_IMPLEMENTATION
#define BAD_USART_IMPLEMENTATION
#define BAD_FLASH_IMPLEMENTATION

#define BAD_EXTI_EXTI0_ISR_IMPLEMENTATION
#define BAD_EXTI_EXTI1_ISR_IMPLEMENTATION
#define BAD_EXTI_EXTI2_ISR_IMPLEMENTATION
#define BAD_EXTI_EXTI3_ISR_IMPLEMENTATION
#define BAD_EXTI_EXTI4_ISR_IMPLEMENTATION
#define BAD_EXTI_EXTI9_5_ISR_IMPLEMENTATION
#define BAD_EXTI_USE_EXTI_5_USER_ISR
#define BAD_EXTI_USE_EXTI_6_USER_ISR
#define BAD_EXTI_USE_EXTI_7_USER_ISR
#define BAD_HARDFAULT_ISR_IMPLEMENTATION
#define BAD_HARDFAULT_USE_UART
#include"badhal.h"

#define UART_GPIO_PORT          (GPIOA)
#define UART1_TX_PIN            (9)
#define UART1_RX_PIN            (10)
#define UART1_TX_AF             (7)
#define UART1_RX_AF             (7)
// Exti pin
#define EXTI_GPIO_PORT          (GPIOA)
#define SYS_CFG_EXTI_TEST_PORT  (SYSCFG_PAx)
#define EXTI0_PIN       (0)
#define EXTI1_PIN       (1)
#define EXTI2_PIN       (2)
#define EXTI3_PIN       (3)
#define EXTI4_PIN       (4)
#define EXTI5_PIN       (5)
#define EXTI6_PIN       (6)
#define EXTI7_PIN       (7)


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

#define BAD_EXTI_TEST_AHB1_PERIPEHRALS    (RCC_AHB1_GPIOA)
#define BAD_EXTI_TEST_APB2_PERIPHERALS    (RCC_APB2_USART1|RCC_APB2_SYSCFGEN)

volatile uint32_t exti0_triggered;
volatile uint32_t exti1_triggered;
volatile uint32_t exti2_triggered;
volatile uint32_t exti3_triggered;
volatile uint32_t exti4_triggered;
volatile uint32_t exti5_triggered;
volatile uint32_t exti6_triggered;
volatile uint32_t exti7_triggered;
void exti0_usr(){
    exti0_triggered++;
}

void exti1_usr(){
    exti1_triggered++;
}

void exti2_usr(){
    exti2_triggered++;
}

void exti3_usr(){
    exti3_triggered++;
}

void exti4_usr(){
    exti4_triggered++;
}

void exti5_usr(){
    exti5_triggered++;
}

void exti6_usr(){
    exti6_triggered++;
}

void exti7_usr(){
    exti7_triggered++;
}

static inline void __main_clock_setup(){
    flash_acceleration_setup(BADHAL_FLASH_LATENCY, FLASH_DCACHE_ENABLE, FLASH_ICACHE_ENABLE);
    rcc_sysclock_setup();
}

static inline void __periph_setup(){
    rcc_set_ahb1_clocking(BAD_EXTI_TEST_AHB1_PERIPEHRALS);
    io_setup_pin(UART_GPIO_PORT, UART1_TX_PIN, MODER_af, UART1_TX_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(UART_GPIO_PORT, UART1_RX_PIN, MODER_af, UART1_RX_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    //Enable UART clocking
    io_setup_pin(EXTI_GPIO_PORT, EXTI0_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, EXTI1_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, EXTI2_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, EXTI3_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, EXTI4_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, EXTI5_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, EXTI6_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(EXTI_GPIO_PORT, EXTI7_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    rcc_set_apb2_clocking(BAD_EXTI_TEST_APB2_PERIPHERALS);
}

static inline void __exti_setup(){
    syscfg_set_exti_pin(SYS_CFG_EXTI_TEST_PORT, EXTI0_PIN);
    syscfg_set_exti_pin(SYS_CFG_EXTI_TEST_PORT, EXTI1_PIN);
    syscfg_set_exti_pin(SYS_CFG_EXTI_TEST_PORT, EXTI2_PIN);
    syscfg_set_exti_pin(SYS_CFG_EXTI_TEST_PORT, EXTI3_PIN);
    syscfg_set_exti_pin(SYS_CFG_EXTI_TEST_PORT, EXTI4_PIN);
    syscfg_set_exti_pin(SYS_CFG_EXTI_TEST_PORT, EXTI5_PIN);
    syscfg_set_exti_pin(SYS_CFG_EXTI_TEST_PORT, EXTI6_PIN);
    syscfg_set_exti_pin(SYS_CFG_EXTI_TEST_PORT, EXTI7_PIN);
    
    exti_configure_line(EXTI0_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(EXTI1_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(EXTI2_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(EXTI3_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(EXTI4_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(EXTI5_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(EXTI6_PIN, EXTI_TRIGGER_BOTH);
    exti_configure_line(EXTI7_PIN, EXTI_TRIGGER_BOTH);
    nvic_enable_interrupt(NVIC_EXTI0_INTR);
    nvic_enable_interrupt(NVIC_EXTI1_INTR);
    nvic_enable_interrupt(NVIC_EXTI2_INTR);
    nvic_enable_interrupt(NVIC_EXTI3_INTR);
    nvic_enable_interrupt(NVIC_EXTI4_INTR);
    nvic_enable_interrupt(NVIC_EXTI9_5_INTR);

}

int main(){
    __DISABLE_INTERUPTS;
    __main_clock_setup();
    __periph_setup();
    __exti_setup();
  
   

    
    __ENABLE_INTERUPTS;

    while(1){

    }
    return 0;
}
