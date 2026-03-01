#ifndef ADAFRUIT_ST7735_API_H
#define ADAFRUIT_ST7735_API_H

#include "stdint.h"
#include "stm32h5xx_hal.h"
#include "stdbool.h"

#include "Adafruit_ST7735_Macros.h"
#include "Adafruit_ST7735.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct ST7735_Handle ST7735_Handle;

//---------------------------------------------------------------------------------
// Adafruit_ST7735.cpp function declarations

ST7735_Handle* ST7735_create(SPI_HandleTypeDef *spiHandle, int16_t cs, GPIO_TypeDef *cs_port, int16_t dc, GPIO_TypeDef *dc_port, int8_t rst);

void ST7735_init(ST7735_Handle* handle, uint8_t options = INITR_GREENTAB);
void setRotation(ST7735_Handle* handle, uint8_t m);
//---------------------------------------------------------------------------------
// Adafruit_ST77xx.cpp function declarations

void setAddrWindow(ST7735_Handle *handle, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void enableDisplay(ST7735_Handle *handle, bool enable);
void enableTearing(ST7735_Handle *handle, bool enable);
void enableSleep(ST7735_Handle *handle, bool enable);

//---------------------------------------------------------------------------------
// Adafruit_SPITFT.cpp function declarations

void drawPixel(ST7735_Handle *handle, int16_t x, int16_t y, uint16_t color);
void fillRect(ST7735_Handle *handle, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawFastHLine(ST7735_Handle *handle, int16_t x, int16_t y, int16_t w, uint16_t color);
void drawFastVLine(ST7735_Handle *handle, int16_t x, int16_t y, int16_t h, uint16_t color);
void invertDisplay(ST7735_Handle *handle, bool i);
uint16_t color565(ST7735_Handle *handle, uint8_t r, uint8_t g, uint8_t b);
void drawRGBBitmap(ST7735_Handle *handle, int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ADAFRUIT_ST7735_API_H