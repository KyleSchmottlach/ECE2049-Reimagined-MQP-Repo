#ifndef ADAFRUIT_TFTSHIELD18_API_H
#define ADAFRUIT_TFTSHIELD18_API_H

#include "stdint.h"
#include "Adafruit_TFTShield18_Macros.h"
#include "stm32h5xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct TFTShield18_Handle TFTShield18_Handle;

void TFTShield18_create(I2C_HandleTypeDef *i2c_handle);

bool TFTShield18_begin(uint8_t addr);
void TFTShield18_setBacklight(uint16_t value);
void TFTShield18_setBacklightFreq(uint16_t freq);
void TFTShield18_tftReset(bool rst);
uint32_t TFTShield18_readButtons();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ADAFRUIT_TFTSHIELD18_API_H