/**
 * @file ili9341.h
 * @brief Header only ILI9341 TFT LCD driver
 *
 * Usage:
 *  - Include this file in your project.
 *  - Define `CRAP_ILI9341_IMPLEMENTATION` in **one** C file to enable
 *    all driver functions.
 *  - Optionally define `CRAP_ILI9341_STATIC` to make all functions `static inline`.
 *  - Optionally define `CRAP_ILI9341_INCLUDE_ISRS` to enable DMA transfer complete ISR support.
 *
 * Example:
 *  #define CRAP_ILI9341_IMPLEMENTATION
 *  #include "ili9341.h"
 *
 *  // Initialize the LCD
 *  ili9341_init();
 *
 *  // Fill the entire screen with a color
 *  ili9341_fill(0xF800); // Red
 *
 *  // Fill a framebuffer region with DMA
 *  uint16_t framebuffer[100*50]; // 100x50 pixels
 *  ili9341_fb_dma_fill(framebuffer, 50, 50, 149, 99);
 *
 *  // Fill a framebuffer region centered on the screen
 *  ili9341_fb_dma_fill_centered(framebuffer, 100, 50);
 *
 *  // Poll until DMA transfer is complete
 *  while (!ili9341_poll_dma_ready());
 */

#pragma once
#ifndef CRAP_ILI9341_H
#define CRAP_ILI9341_H

#include <stdint.h>

#include "nvic.h"

#if defined (CRAP_ILI9341_STATIC) && defined(CRAP_ILI9341_IMPLEMENTATION)
#define CRAP_SPI_STATIC
#define CRAP_DMA_STATIC
#define CRAP_DMA_IMPLEMENTATION
#define CRAP_SPI_IMPLEMENTATION
#endif



#if defined (CRAP_ILI9341_IMPLEMENTATION) && defined (CRAP_ILI9341_INCLUDE_ISRS)
#define CRAP_DMA_DMA2_STREAM2_USE_TC
#define CRAP_DMA_DMA2_STREAM2_ISR_IMPLEMENTATION
#endif


#include "io.h"

#include "spi.h"


#include "dma.h"

#ifdef CRAP_ILI9431_USE_ASSERT
#include "assert.h"
#define ILI9341_ASSERT(x) ASSERT(x)
#else
#define ILI9341_ASSERT(x) 
#endif

#ifdef CRAP_ILI9341_STATIC
#define CRAP_ILI9341_DEF static inline
#else
#define CRAP_ILI9341_DEF extern
#endif




#define ILI9341_SPI                     (SPI1)
#define ILI9341_SPI_FEATURES_CMD        (SPI_FEATURE_MASTER|SPI_FEATURE_PRECALER_div_2|SPI_FEATURE_SOFTWARE_CS|\
                                        SPI_FEATURE_BIDIOE_TX_ONLY|SPI_FEATURE_BIDIRECTIONAL|\
                                        SPI_FEATURE_FRAME_FORMAT_8bit)

#define ILI9341_SPI_FEATURES_DMA        (SPI_FEATURE_MASTER|SPI_FEATURE_PRECALER_div_2|SPI_FEATURE_SOFTWARE_CS|\
                                        SPI_FEATURE_BIDIOE_TX_ONLY|SPI_FEATURE_BIDIRECTIONAL|                  \
                                        SPI_FEATURE_FRAME_FORMAT_16bit|SPI_FEATURE_FORMAT_MSB )
#define ILI9341_SPI_MISC_DMA            (SPI_MISC_ENABLE_DMA_TX)

#define ILI9341_GPIO_PORT               (GPIOB)
#define ILI9341_RESET_PIN               (8)
#define ILI9341_DC_PIN                  (7)
#define ILI9341_CS_PIN                  (6)

#define ILI9341_DMA_STREAM              (DMA_STREAM2)
#define ILI9341_DMA                     (DMA2)
#define ILI9341_DMA_CHANNEL             (DMA_channel2)
#define ILI9341_DMA_SETTINGS_FB         (DMA_feature_DIR_mem_to_periph| DMA_feature_MINC|\
                                        DMA_feature_PSIZE_half_word |\
                                        DMA_feature_MSIZE_word|DMA_feature_MBURST_incr4)
#define ILI9341_DMA_FIFO_SETTINGS_FB    (DMA_FIFO_ENABLE_FIFO|DMA_FIFO_THRESHOLD_4_out_4)
#define ILI9341_DMA_SETTINGS_FILL       (DMA_feature_DIR_mem_to_periph| DMA_feature_PSIZE_half_word |DMA_feature_MSIZE_half_word)
#define ILI9341_NVIC_DMA_INTERRUPT      (NVIC_DMA2_STREAM2_INTR)

#define ILI9341_LCD_HEIGHT              (320)
#define ILI9341_LCD_WIDTH               (240)

CRAP_ILI9341_DEF void ili9341_init(void);
CRAP_ILI9341_DEF void ili9341_fill(uint16_t color);
CRAP_ILI9341_DEF void ili9341_fb_dma_fill_centered(uint16_t* fb, uint16_t width, uint16_t height);
CRAP_ILI9341_DEF void ili9341_fb_dma_fill(uint16_t* fb, uint16_t x_start, uint16_t y_start,uint16_t x_end,uint16_t y_end);
CRAP_ILI9341_DEF uint8_t ili9341_poll_dma_ready();


#ifdef CRAP_ILI9341_IMPLEMENTATION

ALWAYS_INLINE void ili9341_spi_fb_transmition_mode(){
    spi_disable(ILI9341_SPI);
    spi_setup(ILI9341_SPI,ILI9341_SPI_FEATURES_DMA,0,0);
    spi_enable(ILI9341_SPI);
}

ALWAYS_INLINE void ili9341_spi_control_transmition_mode(){
    spi_disable(ILI9341_SPI);;
    spi_setup(ILI9341_SPI,ILI9341_SPI_FEATURES_CMD,0,0);
    spi_enable(ILI9341_SPI);
}

ALWAYS_INLINE void ili9341_select(void)     { io_pin_reset(ILI9341_GPIO_PORT, ILI9341_CS_PIN); }
ALWAYS_INLINE void ili9341_deselect(void)   { io_pin_set(ILI9341_GPIO_PORT, ILI9341_CS_PIN); }
ALWAYS_INLINE void ili9341_enable(void)     { io_pin_set(ILI9341_GPIO_PORT, ILI9341_RESET_PIN); }
ALWAYS_INLINE void ili9341_disable(void)    { io_pin_reset(ILI9341_GPIO_PORT, ILI9341_RESET_PIN); }
ALWAYS_INLINE void ili9341_dc_command(void) { io_pin_reset(ILI9341_GPIO_PORT, ILI9341_DC_PIN); }
ALWAYS_INLINE void ili9341_dc_data(void)    { io_pin_set(ILI9341_GPIO_PORT, ILI9341_DC_PIN); }

#ifdef CRAP_ILI9341_INCLUDE_ISRS

STRONG_USER_ISR(dma2_stream2_tc,uint16_t offset){
    ili9341_deselect();
    ili9341_spi_control_transmition_mode();
} 

#endif


ALWAYS_INLINE void ili9341_spi_init(){
    spi_setup(ILI9341_SPI, ILI9341_SPI_FEATURES_CMD,0, 0);
    spi_enable(ILI9341_SPI);
}




ALWAYS_INLINE void ili9341_spi_start_dma(){
    dma_start_transfer(ILI9341_DMA, ILI9341_DMA_STREAM);
    spi_enable_misc(ILI9341_SPI, SPI_MISC_ENABLE_DMA_TX);
}



ALWAYS_INLINE void ili9341_send_cmd(uint8_t cmd)
{
    ili9341_select();
    ili9341_dc_command();
    spi_transmit_only(ILI9341_SPI,cmd);
    ili9341_deselect();
}

ALWAYS_INLINE void ili9341_send_data(uint8_t data)
{   
    
    ili9341_select();
    ili9341_dc_data();
    spi_transmit_only(ILI9341_SPI,data);
    ili9341_deselect();
}

CRAP_ILI9341_DEF uint8_t ili9341_poll_dma_ready(){
    return dma_stream_n_poll_ready(ILI9341_DMA, ILI9341_DMA_STREAM);
}

CRAP_ILI9341_DEF void ili9341_init(void)
{
    ili9341_enable();


    //SOFTWARE RESET
    ili9341_send_cmd(0x01);
    for (volatile int i = 0; i< 100000; i++);
        
    //POWER CONTROL A
    ili9341_send_cmd(0xCB);
    ili9341_send_data(0x39);
    ili9341_send_data(0x2C);
    ili9341_send_data(0x00);
    ili9341_send_data(0x34);
    ili9341_send_data(0x02);

    //POWER CONTROL B
    ili9341_send_cmd(0xCF);
    ili9341_send_data(0x00);
    ili9341_send_data(0xC1);
    ili9341_send_data(0x30);

    //DRIVER TIMING CONTROL A
    ili9341_send_cmd(0xE8);
    ili9341_send_data(0x85);
    ili9341_send_data(0x00);
    ili9341_send_data(0x78);

    //DRIVER TIMING CONTROL B
    ili9341_send_cmd(0xEA);
    ili9341_send_data(0x00);
    ili9341_send_data(0x00);

    //POWER ON SEQUENCE CONTROL
    ili9341_send_cmd(0xED);
    ili9341_send_data(0x64);
    ili9341_send_data(0x03);
    ili9341_send_data(0x12);
    ili9341_send_data(0x81);

    //PUMP RATIO CONTROL
    ili9341_send_cmd(0xF7);
    ili9341_send_data(0x20);

    //POWER CONTROL,VRH[5:0]
    ili9341_send_cmd(0xC0);
    ili9341_send_data(0x23);

    //POWER CONTROL,SAP[2:0];BT[3:0]
    ili9341_send_cmd(0xC1);
    ili9341_send_data(0x10);

    //VCM CONTROL
    ili9341_send_cmd(0xC5);
    ili9341_send_data(0x3E);
    ili9341_send_data(0x28);

    //VCM CONTROL 2
    ili9341_send_cmd(0xC7);
    ili9341_send_data(0x86);

    //MEMORY ACCESS CONTROL
    ili9341_send_cmd(0x36);
    ili9341_send_data(0x48);

    //PIXEL FORMAT
    ili9341_send_cmd(0x3A);
    ili9341_send_data(0x55);

    //FRAME RATIO CONTROL, STANDARD RGB COLOR
    ili9341_send_cmd(0xB1);
    ili9341_send_data(0x00);
    ili9341_send_data(0x18);

    //DISPLAY FUNCTION CONTROL
    ili9341_send_cmd(0xB6);
    ili9341_send_data(0x08);
    ili9341_send_data(0x82);
    ili9341_send_data(0x27);

    //3GAMMA FUNCTION DISABLE
    ili9341_send_cmd(0xF2);
    ili9341_send_data(0x00);

    //GAMMA CURVE SELECTED
    ili9341_send_cmd(0x26);
    ili9341_send_data(0x01);

    //POSITIVE GAMMA CORRECTION
    ili9341_send_cmd(0xE0);
    ili9341_send_data(0x0F);
    ili9341_send_data(0x31);
    ili9341_send_data(0x2B);
    ili9341_send_data(0x0C);
    ili9341_send_data(0x0E);
    ili9341_send_data(0x08);
    ili9341_send_data(0x4E);
    ili9341_send_data(0xF1);
    ili9341_send_data(0x37);
    ili9341_send_data(0x07);
    ili9341_send_data(0x10);
    ili9341_send_data(0x03);
    ili9341_send_data(0x0E);
    ili9341_send_data(0x09);
    ili9341_send_data(0x00);

    //NEGATIVE GAMMA CORRECTION
    ili9341_send_cmd(0xE1);
    ili9341_send_data(0x00);
    ili9341_send_data(0x0E);
    ili9341_send_data(0x14);
    ili9341_send_data(0x03);
    ili9341_send_data(0x11);
    ili9341_send_data(0x07);
    ili9341_send_data(0x31);
    ili9341_send_data(0xC1);
    ili9341_send_data(0x48);
    ili9341_send_data(0x08);
    ili9341_send_data(0x0F);
    ili9341_send_data(0x0C);
    ili9341_send_data(0x31);
    ili9341_send_data(0x36);
    ili9341_send_data(0x0F);

//EXIT SLEEP
    ili9341_send_cmd(0x11);
    for (volatile int i = 0; i< 120000; i++);
    
    //TURN ON DISPLAY   
    ili9341_send_cmd(0x29);
    for (volatile int i = 0; i< 10000; i++);
}

// ==== Example helper: fill screen ====
CRAP_ILI9341_DEF void ili9341_fill(uint16_t color)
{
    ili9341_send_cmd(0x2A); // column addr set
    ili9341_send_data(0x0); ili9341_send_data(0x00);
    ili9341_send_data(0); ili9341_send_data(0xEF);
    ili9341_send_cmd(0x2B); // row addr set
    ili9341_send_data(0x0); ili9341_send_data(0x0);
    ili9341_send_data(0x1); ili9341_send_data(0x3F);
    ili9341_send_cmd(0x2C); // memory write

    ili9341_select();
    ili9341_dc_data();
    for (uint32_t i = 0; i < (240*320); i++) {
        spi_transmit_only(ILI9341_SPI,color >> 8);
        spi_transmit_only(ILI9341_SPI,color & 0xFF);
    }
    ili9341_deselect();
}

CRAP_ILI9341_DEF void ili9341_fb_dma_fill(uint16_t* fb, uint16_t x_start, uint16_t y_start,uint16_t x_end,uint16_t y_end){
    uint16_t width = (x_end - x_start )+1;
    uint16_t length = (y_end -y_start)+1;
    ILI9341_ASSERT(width*length< UINT16_MAX);
    ILI9341_ASSERT( x_start < ILI9341_LCD_WIDTH && x_end < ILI9341_LCD_WIDTH && y_start < ILI9341_LCD_HEIGHT && y_end< ILI9341_LCD_HEIGHT);
    uint16_t buff_len = width*length;
    nvic_enable_interrupt(ILI9341_NVIC_DMA_INTERRUPT);
    ili9341_send_cmd(0x2A); // column addr set
    ili9341_send_data(x_start>>8); 
    ili9341_send_data(x_start & 0xFF);
    ili9341_send_data(x_end>>8); 
    ili9341_send_data(x_end & 0xFF);
    ili9341_send_cmd(0x2B); // row addr set
    ili9341_send_data(y_start>>8); 
    ili9341_send_data(y_start & 0xFF);
    ili9341_send_data(y_end>>8); 
    ili9341_send_data(y_end & 0xFF);
    ili9341_send_cmd(0x2C); // memory write
    ili9341_spi_fb_transmition_mode();
    dma_setup_transfer(ILI9341_DMA, 
        ILI9341_DMA_STREAM, 
        ILI9341_DMA_CHANNEL, 
        (uint32_t)fb, buff_len,
        (uint32_t)&ILI9341_SPI->DR ,
        DMA_enable_TC, 
        ILI9341_DMA_SETTINGS_FB,
        ILI9341_DMA_FIFO_SETTINGS_FB);
    ili9341_select();
    ili9341_dc_data();
    ili9341_spi_start_dma();
    
}

CRAP_ILI9341_DEF void ili9341_fb_dma_fill_centered(uint16_t* fb, uint16_t width, uint16_t height){
    uint16_t x_start =(( ILI9341_LCD_WIDTH - width)>>1);
    uint16_t x_end = x_start + width - 1;

    uint16_t y_start =((ILI9341_LCD_HEIGHT - height)>>1);
    uint16_t y_end = y_start + height - 1;
    ILI9341_ASSERT(width*height< UINT16_MAX);
    uint16_t buff_len = (width * height);
    nvic_enable_interrupt(ILI9341_NVIC_DMA_INTERRUPT);
    ili9341_send_cmd(0x2A); // column addr set
    ili9341_send_data(x_start>>8); 
    ili9341_send_data(x_start & 0xFF);
    ili9341_send_data(x_end>>8); 
    ili9341_send_data(x_end & 0xFF);
    ili9341_send_cmd(0x2B); // row addr set
    ili9341_send_data(y_start>>8); 
    ili9341_send_data(y_start & 0xFF);
    ili9341_send_data(y_end>>8); 
    ili9341_send_data(y_end & 0xFF);
    ili9341_send_cmd(0x2C); // memory write
    ili9341_spi_fb_transmition_mode();
    dma_setup_transfer(ILI9341_DMA, 
        ILI9341_DMA_STREAM, 
        ILI9341_DMA_CHANNEL, 
        (uint32_t)fb, buff_len,
        (uint32_t)&ILI9341_SPI->DR ,
        DMA_enable_TC, 
        ILI9341_DMA_SETTINGS_FB,
        ILI9341_DMA_FIFO_SETTINGS_FB);
    ili9341_select();
    ili9341_dc_data();
    ili9341_spi_start_dma();
    
}
#endif
#endif