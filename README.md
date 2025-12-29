# BADHAL – Bare-Metal Drivers for STM32

## What is this?  
BADHAL is basically just my personal HAL for the STM32F411CEU6.
Nothing fancy or production-ready, just code I wrote to make my life easier when working with the MCU.

Includes:  
- GPIO - easy pin setup, set/reset pins, configure alternate functions.  
- NVIC - enable/disable interrupts, simple as that.  
- SPI -  SPI setup, DMA support.  
- EXTI  - external interrupts with configurable trigger.  
- Assert (`assert.h`) - prints messages over UART if things go wrong.  
- ILI9341 (`ili9341.h`) - basic LCD driver with DMA framebuffer support.  
- DMA - DMA control and interrupts
- UART - Basic uart stuff
- SYSCFG  - Syscfg, for now only for exti
- Flash - setup latency, caches, and prefetch.
- RCC  - clock configuration 
- Timers - basic timer setup
- Startup (`startup_stm32f411ceu6.c`) - startup file, plain and simple
- Simple linker script (`stm32f411ceu6.ld`)

Most of the drivers don’t cover every feature yet—just what I needed for my projects.
## How to use it  
1. Include the header in your project.  
2. If a driver has a `_IMPLEMENTATION` define, enable it in **one C file**, or use it statically:

```c
#define BAD_GPIO_IMPLEMENTATION
#include "badhal.h"
```
3. Use the function
```c
// Configure PA5 as push-pull output
io_setup_pin(GPIOA, 5,
             MODER_output,
             0, // AF not used
             OSPEEDR_medium_speed,
             PUPDR_no_pull,
             OTYPR_push_pull);

// Toggle pin
io_pin_set(GPIOA, 5);
io_pin_reset(GPIOA, 5);
```
Do note that some of the functions can only be included once and used only in the file its included (io_setup_pin and all the rcc and flash stuff).
Why is it this way? I just find it nicer when all the setup is in one place.

## Notes

If you need multiple settings for your project just do this
```c
#define ILI9341_SPI_FEATURES_CMD                \
                    (SPI_FEATURE_MASTER|        \
                    SPI_FEATURE_PRECALER_div_2| \
                    SPI_FEATURE_SOFTWARE_CS|    \
                    SPI_FEATURE_BIDIOE_TX_ONLY| \
                    SPI_FEATURE_BIDIRECTIONAL|  \
                    SPI_FEATURE_FRAME_FORMAT_8bit)

#define ILI9341_SPI_FEATURES_DMA                \
                (SPI_FEATURE_MASTER|            \
                SPI_FEATURE_PRECALER_div_2|     \
                SPI_FEATURE_SOFTWARE_CS|        \
                SPI_FEATURE_BIDIOE_TX_ONLY|     \
                SPI_FEATURE_BIDIRECTIONAL|      \
                SPI_FEATURE_FRAME_FORMAT_16bit| \
                SPI_FEATURE_FORMAT_MSB )
```
And then just make some helpers to apply them
```c
static inline void ili9341_spi_fb_transmition_mode(){
    spi_disable(ILI9341_SPI);
    spi_setup(ILI9341_SPI,ILI9341_SPI_FEATURES_DMA,0,0);
    spi_enable(ILI9341_SPI);
}

static inline void ili9341_spi_control_transmition_mode(){
    spi_disable(ILI9341_SPI);;
    spi_setup(ILI9341_SPI,ILI9341_SPI_FEATURES_CMD,0,0);
    spi_enable(ILI9341_SPI);
}
``` 

The main(`main.c`) file implements an example that 
- Setups main clock at 100mhz
- Setups apropriate latency / caching
- Setups peripheral clocks
- Setups pins for SPI
- Setups ILI9341 display
- Fills the screen in polling
- Produces a bitmap
- Sends it over dma
- Produces another one and polls till dma is finished
- Repeats the cycle



You can freely modify or copy whatever you need.
