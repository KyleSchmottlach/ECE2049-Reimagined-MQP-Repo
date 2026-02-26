#include "dwt_module.h"

bool DWT_Init() {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable trace
    DWT->CYCCNT = 0; // Reset the cycle counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // Enable the cycle counter
    
    __asm volatile ("nop");
    __asm volatile ("nop");
    __asm volatile ("nop");

    return DWT->CYCCNT != 0;
}

bool DWT_CheckInitialized() {
    return (CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) && (DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk);
}

// void DWT_Delay_us(volatile uint32_t us) {
//     uint32_t startTick = DWT->CYCCNT;
//     uint32_t delayTicks = us * (HAL_RCC_GetHCLKFreq() / 1000000);
//     while ((uint32_t)(DWT->CYCCNT - startTick) < delayTicks);
// }

// void DWT_Delay_ms(volatile uint32_t ms) {
//     uint32_t startTick = DWT->CYCCNT;
//     uint32_t delayTicks = ms * (HAL_RCC_GetHCLKFreq() / 1000);
//     while ((uint32_t)(DWT->CYCCNT - startTick) < delayTicks);
// }