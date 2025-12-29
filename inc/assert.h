/**
 * @file assert.h
 * @brief Header inly assert
 *
 *
 * Features:
 *  - Configurable UART for logging assertion failures (default: USART1)
 *  - Sends human-readable message with line number and file name
 *  - Halts execution in an infinite loop after logging
 *  - Simple macro `ASSERT(x)` for runtime checks
 *
 * Usage:
 *  #define ASSERT_UART USART2   // Optional, override default USART1
 *  #define BAD_ASSERT_IMPLEMENTATION
 *  #include "assert.h"
 *
 *  int x = 5;
 *  ASSERT(x == 10);  // Logs failure via UART and halts
 *
 * Notes:
 *  - The UART is reconfigured with default settings (9600 baud, TX enabled) 
 *    when an assertion fails.
 *  - This is a blocking assertion mechanism; system execution stops on failure.
 *  - Intended for debugging; not recommended for production code in performance-critical paths.
 */



#pragma once
#ifndef BAD_ASSERT_H
#define BAD_ASSERT_H

#include "badhal.h"

#ifndef ASSERT_UART
#define ASSERT_UART USART1
#endif

#define FAULT_LOG_UART_SETTINGS (USART_FEATURE_TRANSMIT_EN)

#define ASSERT(x)                                  \
    do {                                          \
        if (!(x)) {                               \
            assert_failed(__LINE__, __FILE__);    \
        }                                         \
    } while (0)

extern void assert_failed(uint32_t line, char *file);

#ifdef BAD_ASSERT_IMPLEMENTATION

extern void assert_failed(uint32_t line, char *file) {
    uart_disable(ASSERT_UART);
    uart_setup(ASSERT_UART, USART_BRR_9600, FAULT_LOG_UART_SETTINGS, 0, 0);
    uart_enable(ASSERT_UART);
    uart_send_str_polling(ASSERT_UART, "ASSERT FAILED!\r\nLINE:");
    uart_send_dec_unsigned_32bit(ASSERT_UART, line);
    uart_send_str_polling(ASSERT_UART, "FILE:");
    uart_send_str_polling(ASSERT_UART, file);
    uart_send_str_polling(ASSERT_UART, "\r\n");
    while (1);
}

#endif 

#endif 
