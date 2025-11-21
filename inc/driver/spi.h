/**
 * @file spi.h
 * @brief Header only SPI driver
 *
 * - Define `BAD_SPI_IMPLEMENTATION` in **one** C file to enable the
 *    configuration function `io_setup_pin()`.
 * Usage Example:
 *
 * #define MY_SPI_SETTINGS     (SPI_FEATURE_MASTER |\
 *                              SPI_FEATURE_CPOL0 |\
 *                              SPI_FEATURE_CPHA0 |\
 *                              SPI_FEATURE_PRECALER_div_8 |\
 *                              SPI_FEATURE_FORMAT_MSB)
 *  // Configure SPI1 as master, CPOL=0, CPHA=1, MSB first, baud ÷8
 *   spi_disable(SPI1)  
 *   spi_setup(SPI1,MY_SPI_SETTINGS,
 *        SPI_MISC_ENABLE_HARDWARE_NSS,
 *        0);
 *
 *  spi_enable(SPI1);
 *  uint8_t rx = spi_transmit_recieve(SPI1, 0xAA);
 *
 * Notes:
 *  - Many configuration bits in CR1/CR2 may only be modified while SPI is disabled.
 *    Ensure spi_disable() is called before runtime reconfiguration.
 *  - This driver does not configure GPIO pins; ensure MISO, MOSI, SCK, and NSS
 *    are configured correctly using your GPIO module.
 *  - The blocking transfer functions wait for RXNE/BSY flags and are suitable
 *    for simple or low-throughput applications.
 */



#pragma once
#ifndef BAD_SPI_H
#define BAD_SPI_H

#include <stdint.h>

#include "common.h"

#ifdef BAD_SPI_STATIC
    #define BAD_SPI_DEF ALWAYS_INLINE
#else
    #define BAD_SPI_DEF extern
#endif

typedef struct{
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SR;
    __IO uint32_t DR;
    __IO uint32_t CRCPR;
    __IO uint32_t RXCRCR;
    __IO uint32_t TXCRCR;
    __IO uint32_t I2SCFGR;
    __IO uint32_t I2SPR;
}SPI_typedef_t;

#define SPI1_BASE   (0x40013000)
#define SPI1        ((__IO SPI_typedef_t*)SPI1_BASE)

typedef enum {
    SPI_FEATURE_CPHA0   = 0x0,
    SPI_FEATURE_CPHA1   = 0x1,
    SPI_FEATURE_CPOL0   = 0x0,
    SPI_FEATURE_CPOL1   = 0x2, 
    SPI_FEATURE_MASTER  = 0x4,
    SPI_FEATURE_SLAVE   = 0x0,
    SPI_FEATURE_PRECALER_div_2 = 0x0,
    SPI_FEATURE_PRECALER_div_4 = 0x8,
    SPI_FEATURE_PRECALER_div_8 = 0x10,
    SPI_FEATURE_PRECALER_div_16 = 0x18,
    SPI_FEATURE_PRECALER_div_32 = 0x20,
    SPI_FEATURE_PRECALER_div_64 = 0x28,
    SPI_FEATURE_PRECALER_div_128 = 0x30,
    SPI_FEATURE_PRECALER_div_256 = 0x38,
    SPI_FEATURE_FORMAT_MSB = 0x0,
    SPI_FEATURE_FORMAT_LSB = 0x80,
    SPI_FEATURE_HARDWARE_CS = 0x200,
    SPI_FEATURE_SOFTWARE_CS = 0x300,
    SPI_FEATURE_RX_ONLY = 0x400,
    SPI_FEATURE_FRAME_FORMAT_8bit = 0x0,
    SPI_FEATURE_FRAME_FORMAT_16bit = 0x800,
    SPI_FEATURE_ENABLE_CRC = 0x2000,
    SPI_FEATURE_BIDIOE_RX_ONLY = 0,
    SPI_FEATURE_BIDIOE_TX_ONLY = 0x4000,
    SPI_FEATURE_BIDIRECTIONAL = 0x8000 
}SPI_feature_t;

typedef enum{
    SPI_MISC_ENABLE_DMA_RX = 0x1,
    SPI_MISC_ENABLE_DMA_TX = 0x2,
    SPI_MISC_ENABLE_HARDWARE_NSS = 0x4,
    SPI_MISC_MOTOROLA_FORMAT = 0x0,
    SPI_MISC_TI_FORMAT = 0x10,
}SPI_misc_t;

typedef enum{
    SPI_INTERRUPT_ERROR_INTR = 0x20,
    SPI_INTERRUPT_RX_NOT_EMPTY_INTR = 0x40,
    SPI_INTERRUPT_TX_EMPTY_INTR = 0x80
}SPI_interrupt_t;

#define SPI_CR1_SPIEN_MASK  (0x40)
#define SPI_SR_BSY_MASK     (0x80)
#define SPI_SR_RXNE_MASK    (0x1)
#define SPI_SR_TXE_MASK     (0x2)

ALWAYS_INLINE void spi_enable_interrupts(__IO SPI_typedef_t* SPI,SPI_interrupt_t interrupts){
    SPI->CR2 |= interrupts;
}

ALWAYS_INLINE void spi_disable_interrupts(__IO SPI_typedef_t* SPI,SPI_interrupt_t interrupts){
    SPI->CR2 &= ~(interrupts);
}

ALWAYS_INLINE void spi_enable_misc(__IO SPI_typedef_t* SPI, SPI_misc_t misc){ //call this only when spi is disabled
    SPI->CR2 |= misc;
}

ALWAYS_INLINE void spi_disable_misc(__IO SPI_typedef_t* SPI, SPI_misc_t misc){ //call this only when spi is disabled
    SPI->CR2 &= ~misc;
}

ALWAYS_INLINE void spi_setup(__IO SPI_typedef_t* SPI, SPI_feature_t features,SPI_misc_t misc, SPI_interrupt_t interrupts){
    SPI->CR1 = features;
    SPI->CR2 = misc | interrupts;
}

BAD_SPI_DEF void spi_enable(__IO SPI_typedef_t* SPI);
BAD_SPI_DEF void spi_disable(__IO SPI_typedef_t* SPI);
BAD_SPI_DEF uint8_t spi_transmit_recieve(__IO SPI_typedef_t *SPI, uint8_t data);
BAD_SPI_DEF void spi_transmit_only(__IO SPI_typedef_t *SPI, uint8_t data);

#ifdef BAD_SPI_IMPLEMENTATION


BAD_SPI_DEF void spi_enable(__IO SPI_typedef_t* SPI){
    SPI->CR1 |= SPI_CR1_SPIEN_MASK;
    while (!(SPI->CR1 & SPI_CR1_SPIEN_MASK));
    while (SPI->SR & SPI_SR_BSY_MASK); 
}

BAD_SPI_DEF void spi_disable(__IO SPI_typedef_t* SPI){
    while (SPI->SR & SPI_SR_BSY_MASK); 
    SPI->CR1 &= ~(SPI_CR1_SPIEN_MASK);
    while (SPI->CR1 & SPI_CR1_SPIEN_MASK); 
}


BAD_SPI_DEF uint8_t spi_transmit_recieve(__IO SPI_typedef_t *SPI, uint8_t data){
    SPI->DR = data;
    while (!(SPI->SR & SPI_SR_RXNE_MASK));
    return SPI->DR;
} 

BAD_SPI_DEF void spi_transmit_only(__IO SPI_typedef_t *SPI, uint8_t data){;     
    SPI->DR = data;
    while (!(SPI->SR & SPI_SR_BSY_MASK));
    while (SPI->SR & SPI_SR_BSY_MASK); 

}
#endif

#ifdef BAD_SPI_SPI1_ISR_IMPLEMENTATION

#ifdef BAD_SPI_SPI1_USE_RXNE
void spi1_rx_isr(uint16_t data);
#endif

STRONG_ISR(spi1_isr){
    if(SPI1->SR & SPI_SR_RXNE_MASK){
#ifdef BAD_SPI_SPI1_USE_RXNE
        spi1_rx_isr(SPI->DR);
#endif
    }
}


#endif

#endif