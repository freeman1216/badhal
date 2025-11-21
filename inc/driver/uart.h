/**
 * @file uart.h
 * @brief Header only uart driver
 *
 *
 *
 * Usage:
 *  - Include this file and define BAD_USART_IMPLEMENTATION in **one**
 *    C file to enable the UART function definitions.
 *  - Define BAD_USART_USART1_ISR_IMPLEMENTATION and BAD_USART_USART1_USE_RXNE
 *    to implement a receive callback using interrupts.
 *  - 
 * Notes:
 *  - Supports both static inline (`BAD_USART_STATIC`) and external
 *    function declarations.
 *  - Designed for STM32F411CE 
 *  - GTPR,CR2 and most of the CR3 stuff not implemented yet
 *  - uart_setup() just writes whole registers, it is the intended way, 
 *    if you wanna change some settings during runtime use a separate #define with all of the needed settings
 * Example:
 *  #define BAD_USART_IMPLEMENTATION
 *  #define BAD_USART_USART1_ISR_IMPLEMENTATION
 *  #define BAD_USART_USART1_USE_RXNE
 *  #include "uart.h"
 *  #define MY_UART_SETTINGS_FULL_DUPLEX (USART_FEATURE_RECIEVE_EN | USART_FEATURE_TRANSMIT_EN)
 *  #define 
 *  void usart1_rx_isr(char received) {
 *      // Handle received character
 *  }
 *
 *  uart_setup(USART1, USART_BRR_115200,
 *             MY_UART_SETTINGS,
 *             0,
 *             USART_RXNEIE);
 *  
 *  uart_send_str_polling(USART1, "Hello, UART!\r\n");
 *  uart_send_hex_32bit(USART1, 0xDEADBEEF);
 */


#pragma once

#ifndef BAD_UART_H
#define BAD_UART_H
#include <stdint.h>

#include "common.h"

#ifndef BAD_USART_DEF
#ifdef BAD_USART_STATIC
    #define BAD_USART_DEF ALWAYS_INLINE
#else
    #define BAD_USART_DEF extern
#endif
#endif



typedef struct USART_regs_t{
    __IO uint32_t SR;
    __IO uint32_t DR;
    __IO uint32_t BRR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t CR3;
    __IO uint32_t GTPR;
} USART_typedef_t;
// Compile time divisor calculation
#define USART1_BASE (0x40011000)

#define USART1      ((__IO USART_typedef_t *)USART1_BASE)


typedef enum{
    USART_IDLEIE = 0x10,
    USART_RXNEIE = 0x20,
    USART_TCIE = 0x40,
    USART_TXEIE = 0x80,
    USART_PEIE = 0x100
}USART_interrupt_flags_t;


#define USART_CALCULATE_BRR(baud,clock) \
    (((uint16_t)((float)clock/(16*baud)) << 4) | \
     (uint8_t)(((((float)clock/(16*baud)) - (uint16_t)((float)clock/(16*baud)))*16) + 0.5f))


#define USART_BRR_115200 USART_CALCULATE_BRR(115200UL,CLOCK_SPEED)
#define USART_BRR_9600 USART_CALCULATE_BRR(9600UL,CLOCK_SPEED)
#define USART_CR1_USART_ENABLE 0x2000
typedef enum{
    USART_SR_PE     = 0x1,
    USART_SR_FE     = 0x2,
    USART_SR_NF     = 0x4,
    USART_SR_ORE    = 0x8,
    USART_SR_IDLE   = 0x10,
    USART_SR_RXNE   = 0x20,
    USART_SR_TC     = 0x40,
    USART_SR_TXE    = 0x80,
    USART_SR_TBD    = 0x100,
    USART_SR_CTS    = 0x200
}USART_SR_flags_t;

typedef enum{
    USART_FEATURE_RECIEVE_EN = 0x4,
    USART_FEATURE_TRANSMIT_EN = 0x8,
    USART_FEATURE_PARITY_EVEN = 0x0,
    USART_FEATURE_PARITY_ODD = 0x200,
    USART_FEATURE_PARITY_OFF = 0x0,
    USART_FEATURE_PARITY_ON = 0x400,
    USART_FEATURE_WAKE_IDLE = 0x0,
    USART_FEATURE_WAKE_ADDR_MART =0x800,  
    USART_FEATURE_8BIT_WORD = 0x0,
    USART_FEATURE_9BIT_WORD = 0x1000,
    USART_FEATURE_OVERSAMPLING_16 =0x0, 
    USART_FEATURE_OVERSAMPLING_8 = 0x8000
}USART_feature_t;

typedef enum{
    USART_MISC_DMA_RECIEVE = 0x40, 
    USART_MISC_DMA_TRANSMIT = 0x80
}USART_misc_t;


ALWAYS_INLINE void uart_enable_misc(__IO USART_typedef_t * USART , USART_misc_t misc){
    USART->CR3 |= misc;
}

ALWAYS_INLINE void uart_disable_misc(__IO USART_typedef_t * USART ,USART_misc_t misc){
    USART->CR3 &= ~(misc);
}

ALWAYS_INLINE void uart_enable_interrupts(__IO USART_typedef_t USART,USART_interrupt_flags_t interrupts){
    USART1->CR1 |= interrupts;
}

ALWAYS_INLINE void uart_disable_interrupts(__IO USART_typedef_t USART,USART_interrupt_flags_t interrupts){
    USART1->CR1 &= ~(interrupts);
}
BAD_USART_DEF void uart_enable(__IO USART_typedef_t* USART);
BAD_USART_DEF void uart_disable(__IO USART_typedef_t * USART);
BAD_USART_DEF void uart_putchar_polling(__IO USART_typedef_t*,char);
BAD_USART_DEF char uart_getchar_polling(__IO USART_typedef_t*);
BAD_USART_DEF void uart_setup(__IO USART_typedef_t * USART,
    uint16_t BRR,
    USART_feature_t features,
    USART_misc_t misc,
    USART_interrupt_flags_t interrupt);
BAD_USART_DEF void uart_send_str_polling(__IO USART_typedef_t* USART ,const char* str);
BAD_USART_DEF void uart_send_hex_32bit(__IO USART_typedef_t* USART,uint32_t value);
BAD_USART_DEF void uart_send_dec_unsigned_32bit(__IO USART_typedef_t *USART ,uint32_t value);

#ifdef BAD_USART_IMPLEMENTATION

BAD_USART_DEF void uart_enable(__IO USART_typedef_t* USART){
    while (!(USART->SR & USART_SR_TC));
    USART->CR1 |= USART_CR1_USART_ENABLE;
}
BAD_USART_DEF void uart_disable(__IO USART_typedef_t * USART) {
    while (!(USART->SR & USART_SR_TC));
    USART->CR1 &= ~USART_CR1_USART_ENABLE;
}

BAD_USART_DEF void uart_putchar_polling(__IO USART_typedef_t* USART,char ch){
    while (!(USART->SR & USART_SR_TXE)); 
    USART->DR = ch;
}

BAD_USART_DEF char uart_getchar_polling(__IO USART_typedef_t* USART){
    while(!(USART->SR & USART_SR_RXNE));
    return (char)USART->DR;
}

BAD_USART_DEF void uart_setup(__IO USART_typedef_t * USART,
    uint16_t BRR,
    USART_feature_t features,
    USART_misc_t misc,
    USART_interrupt_flags_t interrupts)
{
    USART->CR1 = features| interrupts;
    USART->BRR = BRR;
    USART->CR2 = 0; //unsupported for now
    USART->CR3 = misc;
}
BAD_USART_DEF void uart_send_str_polling(__IO USART_typedef_t* USART ,const char* str){
    while(*str){
        uart_putchar_polling(USART,*str);
        str++;
    }   
}

BAD_USART_DEF void uart_send_hex_32bit(__IO USART_typedef_t* USART,uint32_t value){
    const char lookup[] ="0123456789ABCDEF";
    
    for (uint8_t i = 0; i < 8 ;i++){
        uint32_t idx = (value >>28) & 0xF ;
        char c = lookup[idx];
        uart_putchar_polling(USART,c);
        value <<= 4;
    }
    uart_send_str_polling(USART, "\r\n");
}

BAD_USART_DEF void uart_send_dec_unsigned_32bit(__IO USART_typedef_t *USART ,uint32_t value){
    char buff[11];
    uint8_t idx = 0;
    
    do{ 
        buff[idx] = (value%10)+'0';
        value/=10;
        idx++;
    }
    while (value!=0);
    
    idx --;

    for(;idx!=0xFF;idx--){
        uart_putchar_polling(USART,buff[idx]);
    }
    uart_send_str_polling(USART, "\r\n");
}
#endif


#ifdef BAD_USART_USART1_ISR_IMPLEMENTATION
#ifdef BAD_USART_USART1_USE_RXNE
void usart1_rx_isr(char);
#endif

STRONG_ISR(usart1_isr){
    if(USART1->SR & RXNE_MASK){
#ifdef BAD_USART_USART1_USE_RXNE
        usart1_rx_isr(USART1->DR);
#endif
    }
}

#endif

#endif