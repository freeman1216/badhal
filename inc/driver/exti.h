/**
 * @file exti.h
 * @brief Header only driver for exti
 *
 *
 *
 * Usage Example:
 
 *  // Configure EXTI line 5 for both edges
 *  exti_configure_line(5, EXTI_TRIGGER_BOTH);
 *
 *  // Make sure you also configure SYSCFG and NVIC!
 *
 * Notes:
 *  - This module configures only the EXTI peripheral.
 *    You must map the GPIO pin to the EXTI line using SYSCFG_EXTICR.
 *  - You must enable the interrupt in NVIC separately.
 */

#pragma once 
#ifndef BAD_EXTI
#define BAD_EXTI

#include "stdint.h"

#include "common.h"

#ifdef BAD_EXTI_STATIC
    #define BAD_EXTI_DEF static inline
#else
    #define BAD_EXTI_DEF extern
#endif

typedef struct{
    __IO uint32_t IMR;
    __IO uint32_t EMR;
    __IO uint32_t RTSR;
    __IO uint32_t FTSR;
    __IO uint32_t SWIER;
    __IO uint32_t PR;
} EXTI_typedef_t;

typedef enum {
    EXTI_TRIGGER_RISING  = 1,
    EXTI_TRIGGER_FALLING = 2,
    EXTI_TRIGGER_BOTH = EXTI_TRIGGER_RISING | EXTI_TRIGGER_FALLING, // 0x3
} EXTI_trigger_t;

#define EXTI_BASE   (0x40013C00UL)
#define EXTI        ((__IO EXTI_typedef_t *)EXTI_BASE)
BAD_EXTI_DEF void exti_configure_line(uint8_t line, EXTI_trigger_t trigger);
#ifdef BAD_EXTI_IMPLEMENTATION

BAD_EXTI_DEF void exti_configure_line(uint8_t line, EXTI_trigger_t trigger)
{
    
    EXTI->RTSR &= ~(1 << line);
    EXTI->FTSR &= ~(1 << line);

    
    if (trigger & EXTI_TRIGGER_RISING)
        EXTI->RTSR |= (1 << line);
    if (trigger & EXTI_TRIGGER_FALLING)
        EXTI->FTSR |= (1 << line);

    // Unmask the interrupt
    EXTI->PR  |= (1 << line);
    EXTI->IMR |= (1 << line);
}

#endif

#ifdef BAD_EXTI_EXTI0_ISR_IMPLEMENTATION

void exti0_usr();

#define EXTI_PR_EXTI0 (0x1)

STRONG_ISR(exti0_isr){
    EXTI->PR = EXTI_PR_EXTI0;
    exti0_usr();
}

#endif

#ifdef BAD_EXTI_EXTI1_ISR_IMPLEMENTATION

void exti1_usr();

#define EXTI_PR_EXTI1 (0x2)

STRONG_ISR(exti1_isr){
    EXTI->PR = EXTI_PR_EXTI1;
    exti1_usr();
}

#endif

#ifdef BAD_EXTI_EXTI2_ISR_IMPLEMENTATION

void exti2_usr();

#define EXTI_PR_EXTI2 (0x4)

STRONG_ISR(exti2_isr){
    EXTI->PR = EXTI_PR_EXTI2;
    exti2_usr();
}

#endif

#ifdef BAD_EXTI_EXTI3_ISR_IMPLEMENTATION

void exti3_usr();

#define EXTI_PR_EXTI3 (0x8)

STRONG_ISR(exti3_isr){
    EXTI->PR = EXTI_PR_EXTI3;
    exti3_usr();
}

#endif

#ifdef BAD_EXTI_EXTI4_ISR_IMPLEMENTATION

void exti4_usr();

#define EXTI_PR_EXTI4 (0x10)

STRONG_ISR(exti4_isr){
    EXTI->PR = EXTI_PR_EXTI4;
    exti4_usr();
}

#endif
#ifdef BAD_EXTI_EXTI9_5_ISR_IMPLEMENTATION

#ifdef BAD_EXTI_USE_EXTI_5_USER_ISR
void exti5_usr();
#endif
#ifdef BAD_EXTI_USE_EXTI_6_USER_ISR
void exti6_usr();
#endif
#ifdef BAD_EXTI_USE_EXTI_7_USER_ISR
void exti7_usr();
#endif
#ifdef BAD_EXTI_USE_EXTI_8_USER_ISR
void exti8_usr();
#endif
#ifdef BAD_EXTI_USE_EXTI_9_USER_ISR
void exti9_usr();
#endif

#define EXTI_PR_EXTI9_5_mask (0x3E0)

STRONG_ISR(exti9_5_isr){
    enum EXTI9_5_masks{
        EXTI_PR_EXTI5 = 0x20,
        EXTI_PR_EXTI6 = 0x40,
        EXTI_PR_EXTI7 = 0x80,
        EXTI_PR_EXTI8 = 0x100,
        EXTI_PR_EXTI9 = 0x200
    };

    uint32_t pending = EXTI->PR & (EXTI_PR_EXTI9_5_mask);

    if(pending & EXTI_PR_EXTI5){
        EXTI->PR = EXTI_PR_EXTI5;
#ifdef BAD_EXTI_USE_EXTI_5_USER_ISR        
        exti5_usr();
#endif
    }

    if(pending & EXTI_PR_EXTI6){
        EXTI->PR = EXTI_PR_EXTI6;
#ifdef BAD_EXTI_USE_EXTI_6_USER_ISR
        exti6_usr();
#endif
    }

    if(pending & EXTI_PR_EXTI7){
        EXTI->PR = EXTI_PR_EXTI7;
#ifdef BAD_EXTI_USE_EXTI_7_USER_ISR
        exti7_usr();
#endif
    }

    if(pending & EXTI_PR_EXTI8){
        EXTI->PR = EXTI_PR_EXTI8;
#ifdef BAD_EXTI_USE_EXTI_8_USER_ISR
        exti8_usr();
#endif
    }

    if(pending & EXTI_PR_EXTI9){
        EXTI->PR = EXTI_PR_EXTI9;
#ifdef BAD_EXTI_USE_EXTI_9_USER_ISR
        exti9_usr();
#endif
    }
}
#endif
#endif