/**
* Functions for using the DWT (Data Watchpoint and Trace) unit to create accurate delays.
* Useful for applications requiring stricter time requirements and accuracy than HAL_Delay
* 
* Functions adapted from https://deepbluembedded.com/stm32-delay-microsecond-millisecond-utility-dwt-delay-timer-delay/
*/

#ifndef DWT_MODULE_H
#define DWT_MODULE_H

#include "stdint.h"
#include "stm32h5xx_nucleo.h"
#include "stm32h5xx_hal.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

bool DWT_Init(void);
bool DWT_CheckInitialized(void);

inline void DWT_delayUs(volatile uint32_t us) {
    uint32_t startTick = DWT->CYCCNT;
    uint32_t delayTicks = us * (HAL_RCC_GetHCLKFreq() / 1000000);
    while ((uint32_t)(DWT->CYCCNT - startTick) < delayTicks);
}

inline void DWT_delayMs(volatile uint32_t ms) {
    uint32_t startTick = DWT->CYCCNT;
    uint32_t delayTicks = ms * (HAL_RCC_GetHCLKFreq() / 1000);
    while ((uint32_t)(DWT->CYCCNT - startTick) < delayTicks);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // DWT_MODULE_H