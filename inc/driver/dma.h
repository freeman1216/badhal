/**
 * @file dma.h
 * @brief Header only DMA driver
 *
 * 
 *
 * Usage:
 *  - Include this file and define CRAP_DMA_IMPLEMENTATION in **one**
 *    C file to enable the `dma_setup_transfer` function definition.
 *  - Define CRAP_DMA_DMA2_STREAM2_ISR_IMPLEMENTATION to provide
 *    the DMA2 Stream2 interrupt service routine with optional callbacks.
 *    Then define which callbacks to use (e.g., CRAP_DMA_DMA2_STREAM2_USE_FE,
 *    USE_TC, USE_DME, USE_HT, USE_TE) and implement your callback functions.
 *  
 * Notes:
 *  - Supports both static inline (`CRAP_DMA_STATIC`) and external
 *    function declarations.
 *  - Designed for STM32F411CE, but may work with other STM32F4 devices.
 *
 * Example:
 *  #define CRAP_DMA_IMPLEMENTATION
 *  #define CRAP_DMA_DMA2_STREAM2_ISR_IMPLEMENTATION
 *  #define CRAP_DMA_DMA2_STREAM2_USE_TC
 *  #include "dma.h"
 *
 *  #define MY_DMA_INTERRUPTS   (DMA_enable_TC)
 *  #define MY_DMA_SETTINGS     (DMA_feature_DIR_mem_to_periph | DMA_feature_MINC | DMA_feature_CIRC | \
 *                               DMA_FIFO_ENABLE_FIFO | DMA_FIFO_THRESHOLD_4_out_4)
 *
 *  void dma2_stream2_tc(uint16_t offset) {
 *      // Your callback implementation
 *  }
 *
 *  dma_setup_transfer(DMA2, DMA_STREAM2, DMA_channel3, mem_addr, buf_len, periph_addr,
 *                     MY_DMA_INTERRUPTS, MY_DMA_SETTINGS);
 *  dma_start_transfer(DMA2, DMA_STREAM2);
 */



#pragma once
#ifndef CRAP_DMA_H
#define CRAP_DMA_H
#include <stdint.h>

#include "common.h"

#ifdef CRAP_DMA_STATIC
    #define CRAP_DMA_DEF static inline
#else
    #define CRAP_DMA_DEF extern
#endif

typedef struct{
    __IO uint32_t CR;
    __IO uint32_t NDTR;
    __IO uint32_t PAR;
    __IO uint32_t M0AR;
    __IO uint32_t M1AR;
    __IO uint32_t FCR;
}DMA_stream_typedef_t;

typedef struct {
    __IO uint32_t LISR;
    __IO uint32_t HISR;
    __IO uint32_t LIFCR;
    __IO uint32_t HIFCR;
    __IO DMA_stream_typedef_t streams[8];
} DMA_typedef_t ;

typedef enum{
    DMA_STREAM0 = 0,
    DMA_STREAM1, 
    DMA_STREAM2,  
    DMA_STREAM3,
    DMA_STREAM4,
    DMA_STREAM5,
    DMA_STREAM6,
    DMA_STREAM7  
} DMA_stream_num_t;

typedef enum {
    DMA_enable_DME = 0x2,
    DMA_enable_TE  = 0x4,
    DMA_enable_HT  = 0x8,
    DMA_enable_TC  = 0x10
} DMA_interrupts_t;

typedef enum{
    DMA_clear_FE  = 0x1,
    DMA_clear_DME = 0x4,
    DMA_clear_TE  = 0x8,
    DMA_clear_HT  = 0x10,
    DMA_clear_TC  = 0x20,
    DMA_clear_all = DMA_clear_DME | DMA_clear_FE | DMA_clear_HT | DMA_clear_TE | DMA_clear_TC
} DMA_clear_interrupts_t;

typedef enum{
    DMA_feature_PFCTRL = 0x20,
    DMA_feature_DIR_periph_to_mem = 0x0,
    DMA_feature_DIR_mem_to_periph = 0x40,
    DMA_feature_DIR_mem_to_mem = 0x80,
    DMA_feature_CIRC = 0x100,
    DMA_feature_PINC = 0x200,
    DMA_feature_MINC = 0x400,
    DMA_feature_PSIZE_byte = 0x0,
    DMA_feature_PSIZE_half_word = 0x800,
    DMA_feature_PSIZE_word = 0x1000,
    DMA_feature_MSIZE_byte = 0x0,
    DMA_feature_MSIZE_half_word = 0x2000,
    DMA_feature_MSIZE_word = 0x4000,
    DMA_feature_PINCOS_pinc = 0x0,
    DMA_feature_PINCOS_word = 0x8000,
    DMA_feature_PL_low_prio = 0x0,
    DMA_feature_PL_medium_prio = 0x10000,
    DMA_feature_PL_high_prio = 0x20000,
    DMA_feature_PL_very_high_prio = 0x30000,
    DMA_feature_DBM = 0x40000,
    DMA_feature_CT = 0x80000,
    DMA_feature_PBURST_no_burst = 0x0,
    DMA_feature_PBURST_incr4 = 0x200000,
    DMA_feature_PBURST_incr8 = 0x400000,
    DMA_feature_PBURST_incr16 = 0x600000,
    DMA_feature_MBURST_no_burst = 0x0,
    DMA_feature_MBURST_incr4 = 0x800000,
    DMA_feature_MBURST_incr8 = 0x1000000,
    DMA_feature_MBURST_incr16 = 0x2000000,
}DMA_features_t;

typedef enum {
    DMA_channel0 = 0,
    DMA_channel1 = (1<<25),
    DMA_channel2 = (2<<25),
    DMA_channel3 = (3<<25),
    DMA_channel4 = (4<<25),
    DMA_channel5 = (5<<25),
    DMA_channel6 = (6<<25),
    DMA_channel7 = (7<<25)
}DMA_channel_num_t;

typedef enum{
    DMA_FIFO_THRESHOLD_1_out_4 = 0x0,
    DMA_FIFO_THRESHOLD_2_out_4 = 0x1,
    DMA_FIFO_THRESHOLD_3_out_4 = 0x2,
    DMA_FIFO_THRESHOLD_4_out_4 = 0x3,
    DMA_FIFO_ENABLE_FIFO = 0x4,
    DMA_FIFO_ENABLE_ERROR_INTERRUPT = 0x80
}DMA_fifo_settings_t;
#define DMA1_BASE   (0x40026000)
#define DMA2_BASE   (0x40026400)

#define DMA1 ((__IO DMA_typedef_t *) DMA1_BASE)
#define DMA2 ((__IO DMA_typedef_t *) DMA2_BASE)

#define CR_EN_MASK  (0x1)

ALWAYS_INLINE void dma_clear_interrupts(__IO DMA_typedef_t * DMA,DMA_stream_num_t stream, DMA_clear_interrupts_t interrupts){
    static const uint32_t shift[4] = {
        0,
        6,
        16,
        22
    };

    if (stream <= 3){
        DMA->LIFCR = interrupts << shift[stream];
    }
    else{
        DMA->HIFCR = interrupts << shift[stream - 4];
    }
}

ALWAYS_INLINE uint8_t dma_stream_n_poll_ready(__IO DMA_typedef_t * DMA,DMA_stream_num_t stream){
    return DMA->streams[stream].NDTR == 0;
}

ALWAYS_INLINE void dma_enable_interrupts(__IO DMA_typedef_t * DMA,DMA_stream_num_t stream,DMA_interrupts_t interrupts){
    DMA->streams[stream].CR |= interrupts;
}
ALWAYS_INLINE void dma_disable_interrupts(__IO DMA_typedef_t * DMA,DMA_stream_num_t stream, DMA_interrupts_t interrupts){
    DMA->streams[stream].CR &= ~(interrupts);
}

ALWAYS_INLINE void dma_start_transfer(__IO DMA_typedef_t * DMA, DMA_stream_num_t stream){
    DMA->streams[stream].CR |= CR_EN_MASK;
}

CRAP_DMA_DEF void dma_setup_transfer(__IO DMA_typedef_t * DMA, 
    DMA_stream_num_t stream,
    DMA_channel_num_t channel,volatile uint32_t mem,
    uint16_t bufflen,
    uint32_t periph, 
    DMA_interrupts_t interrupts, 
    DMA_features_t features,
    DMA_fifo_settings_t fifo_settings);



#ifdef CRAP_DMA_IMPLEMENTATION

CRAP_DMA_DEF void dma_setup_transfer(__IO DMA_typedef_t * DMA, DMA_stream_num_t stream,DMA_channel_num_t channel,volatile uint32_t mem,uint16_t bufflen,uint32_t periph, DMA_interrupts_t interrupts, DMA_features_t features,DMA_fifo_settings_t fifo_settings){
    DMA->streams[stream].CR &= ~(CR_EN_MASK);
    while(DMA->streams[stream].CR & CR_EN_MASK);
    dma_clear_interrupts(DMA, stream, DMA_clear_all);
    DMA->streams[stream].PAR = (uint32_t)periph;
    DMA->streams[stream].M0AR = (uint32_t)mem;
    DMA->streams[stream].NDTR = bufflen;
    DMA->streams[stream].CR = interrupts | features | channel;
    DMA->streams[stream].FCR = fifo_settings;
}

#endif



#ifdef CRAP_DMA_DMA2_STREAM2_ISR_IMPLEMENTATION

#ifdef CRAP_DMA_DMA2_STREAM2_USE_FE
void dma2_stream2_fe(uint16_t offset);
#endif

#ifdef CRAP_DMA_DMA2_STREAM2_USE_DME
void dma2_stream2_dme(uint16_t offset);
#endif

#ifdef CRAP_DMA_DMA2_STREAM2_USE_TE
void dma2_stream2_te(uint16_t offset);
#endif

#ifdef CRAP_DMA_DMA2_STREAM2_USE_TC
void dma2_stream2_tc(uint16_t offset);
#endif

#ifdef CRAP_DMA_DMA2_STREAM2_USE_HT
void dma2_stream2_ht(uint16_t offset);
#endif


STRONG_ISR(dma2_stream2_isr){
    enum DMA_Stream2_flag_LISR {
        DMA_Stream2_frame_error  = (1UL << 16UL),
        DMA_Stream2_direct_mode_error= (1UL << 18UL),
        DMA_Stream2_transfer_error  = (1UL << 19UL),
        DMA_Stream2_half_transfer  = (1UL << 20UL),
        DMA_Stream2_transfer_complete  = (1UL << 21UL)
    };
    
    if(DMA2->LISR & DMA_Stream2_frame_error){
        
        DMA2->LIFCR |= DMA_Stream2_frame_error;
#ifdef CRAP_DMA_DMA2_STREAM2_USE_FE
        dma2_stream2_fe(DMA2->streams[2].NDTR);
#endif
    }

    if(DMA2->LISR & DMA_Stream2_direct_mode_error){
        DMA2->LIFCR |= DMA_Stream2_direct_mode_error;
#ifdef CRAP_DMA_DMA2_STREAM2_USE_DME
        dma2_stream2_dme(DMA2->streams[2].NDTR);
#endif
    }

    if(DMA2->LISR & DMA_Stream2_transfer_error){
        DMA2->LIFCR |= DMA_Stream2_transfer_error;
#ifdef CRAP_DMA_DMA2_STREAM2_USE_TE
        dma2_stream2_te(DMA2->streams[2].NDTR);
#endif
    }

    if(DMA2->LISR & DMA_Stream2_transfer_complete){
        
        DMA2->LIFCR|= DMA_Stream2_transfer_complete;
#ifdef CRAP_DMA_DMA2_STREAM2_USE_TC
        dma2_stream2_tc(DMA2->streams[2].NDTR);
#endif
    }

    if(DMA2->LISR & DMA_Stream2_half_transfer){
        DMA2->LIFCR |= DMA_Stream2_half_transfer;
#ifdef CRAP_DMA_DMA2_STREAM2_USE_HT
        dma2_stream2_ht(DMA2->streams[2].NDTR);
#endif
    }
}
#endif


#endif