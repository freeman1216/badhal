#include <stdint.h>

#include "common.h"
#include "nvic.h"

#define CRAP_RCC_IMPLEMENTATION
#include "rcc.h"
#include "flash.h"

#define CRAP_IO_IMPLEMENTATION
#include "io.h"


#define UART_GPIO_PORT          (GPIOA)
#define UART1_TX_PIN            (9)
#define UART1_RX_PIN            (10)
#define UART1_TX_AF             (7)
#define UART1_RX_AF             (7)

//spi pins
#define SPI_GPIO_PORT       (GPIOB)
#define SPI_SCK_PIN         (3)
#define SPI_MISO_PIN        (4)
#define SPI_MOSI_PIN        (5)
#define SPI_SCK_AF          (5)
#define SPI_MISO_AF         (5)
#define SPI_MOSI_AF         (5)
#define ILI9431_GPIO_PORT   (GPIOB)
#define ILI9431_RESET       (8)
#define ILI9341_DC          (7)
#define ILI9341_CS          (6)
// Exti pin
#define EXTI1_GPIO_PORT (GPIOB)
#define EXTI1_PIN       (1)

#define CRAP_USART_IMPLEMENTATION
#include "uart.h"
#define CRAP_ASSERT_IMPLEMENTATION
#define CRAP_ILI9341_STATIC
#define CRAP_ILI9341_INCLUDE_ISRS
#define CRAP_ILI9341_IMPLEMENTATION
#define CRAP_ILI9431_USE_ASSERT
#include "ili9341.h"


#define EXTI_PORT   (SYSCFG_PBx)
#define EXTI_PIN    (1)

uint16_t random_bitmap[240*240];

    // HSE  = 25
    // PLLM = 25
    // PLLN = 400
    // PLLQ = 10
    // PLLP = 4
    // Sysclock = 100
#define CRAPGB_PLLM (25)
#define CRAPGB_PLLN (400)
#define CRAPGB_PLLQ (10)
#define CRAPGB_FLASH_LATENCY (FLASH_LATENCY_3ws)

#define CRAP_GB_AHB1_PERIPEHRALS    (RCC_AHB1_GPIOA|RCC_AHB1_DMA2|RCC_AHB1_GPIOB)
#define CRAP_GB_APB2_PERIPHERALS    (RCC_APB2_USART1|RCC_APB2_SPI1|RCC_APB2_SYSCFGEN)

static inline void __main_clock_setup(){
    rcc_enable_hse();
    rcc_pll_setup( PLLP4, CRAPGB_PLLM, CRAPGB_PLLN, CRAPGB_PLLQ, PLL_SOURCE_HSE);
    rcc_bus_prescalers_setup(HPRE_DIV_1, PPRE_DIV_2, PPRE_DIV_1);
    flash_acceleration_setup(CRAPGB_FLASH_LATENCY, FLASH_DCACHE_ENABLE, FLASH_ICACHE_ENABLE);
    rcc_enable_and_switch_to_pll();
}

static inline void __periph_setup(){
    rcc_set_ahb1_clocking(CRAP_GB_AHB1_PERIPEHRALS);
    io_setup_pin(UART_GPIO_PORT, UART1_TX_PIN, MODER_af, UART1_TX_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(UART_GPIO_PORT, UART1_RX_PIN, MODER_af, UART1_RX_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    //Enable UART clocking
    io_setup_pin(SPI_GPIO_PORT, SPI_SCK_PIN, MODER_af, SPI_SCK_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(SPI_GPIO_PORT, SPI_MISO_PIN, MODER_af, SPI_MISO_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(SPI_GPIO_PORT, SPI_MOSI_PIN, MODER_af, SPI_MOSI_AF, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(ILI9431_GPIO_PORT, ILI9341_CS, MODER_output, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(ILI9431_GPIO_PORT, ILI9431_RESET, MODER_output, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    io_setup_pin(ILI9431_GPIO_PORT, ILI9341_DC, MODER_output, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    //io_setup_pin(EXTI1_GPIO_PORT, EXTI1_PIN, MODER_reset_input, 0, OSPEEDR_high_speed, PUPDR_no_pull, OTYPR_push_pull);
    rcc_set_apb2_clocking(CRAP_GB_APB2_PERIPHERALS);
}

int main(){
    __DISABLE_INTERUPTS;
    __main_clock_setup();
    __periph_setup();
   
    ili9341_spi_init();
   

    
    __ENABLE_INTERUPTS;
    ili9341_init();   
    ili9341_fill(0x0000);
    uint16_t frame = 0;
    while(1){
        for (uint16_t y = 0; y < 240; y++) {
            for (uint16_t x = 0; x < 240; x++) {
                uint16_t r = ((x+frame) >> 3) & 0x1F;         
                uint16_t g = ((y+frame) >> 2) & 0x3F;          
                uint16_t b = ((x ^ y) >> 3) & 0x1F;    
                random_bitmap[y * 240 + x] = (r << 11) | (g << 5) | b;
            }
        }
        frame++;
        while(!ili9341_poll_dma_ready());
        ili9341_fb_dma_fill_centered(random_bitmap, 240, 240);
    
        
    }
    return 0;
}