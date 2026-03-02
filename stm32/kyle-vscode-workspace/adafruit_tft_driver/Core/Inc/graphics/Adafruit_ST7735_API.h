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
void ST7735_setRotation(ST7735_Handle* handle, uint8_t m);
//---------------------------------------------------------------------------------
// Adafruit_ST77xx.cpp function declarations

void ST7735_setAddrWindow(ST7735_Handle *handle, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void ST7735_enableDisplay(ST7735_Handle *handle, bool enable);
void ST7735_enableTearing(ST7735_Handle *handle, bool enable);
void ST7735_enableSleep(ST7735_Handle *handle, bool enable);

//---------------------------------------------------------------------------------
// Adafruit_SPITFT.cpp function declarations

void ST7735_drawPixel(ST7735_Handle *handle, int16_t x, int16_t y, uint16_t color);
void ST7735_fillRect(ST7735_Handle *handle, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ST7735_drawFastHLine(ST7735_Handle *handle, int16_t x, int16_t y, int16_t w, uint16_t color);
void ST7735_drawFastVLine(ST7735_Handle *handle, int16_t x, int16_t y, int16_t h, uint16_t color);
void ST7735_invertDisplay(ST7735_Handle *handle, bool i);
uint16_t ST7735_color565(ST7735_Handle *handle, uint8_t r, uint8_t g, uint8_t b);
void ST7735_drawRGBBitmap(ST7735_Handle *handle, int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h, uint8_t *mask = NULL, bool useMask = false);

//---------------------------------------------------------------------------------
// Adafruit_GFX.cpp function declarations
void ST7735_fillScreen(ST7735_Handle *handle, uint16_t color);
void ST7735_drawLine(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void ST7735_drawRect(ST7735_Handle *handle, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

void ST7735_drawCircle(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t r, uint16_t color);
void ST7735_drawCircleHelper(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void ST7735_fillCircle(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t r, uint16_t color);
void ST7735_fillCircleHelper(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void ST7735_drawEllipse(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t rw, int16_t rh, uint16_t color);
void ST7735_fillEllipse(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t rw, int16_t rh, uint16_t color);
void ST7735_drawTriangle(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void ST7735_fillTriangle(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void ST7735_drawRoundRect(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
void ST7735_fillRoundRect(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
void ST7735_drawBitmap(ST7735_Handle *handle, int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg = 0, bool drawBg = false);
void ST7735_drawXBitmap(ST7735_Handle *handle, int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void ST7735_drawGrayscaleBitmap(ST7735_Handle *handle, int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint8_t *mask = NULL, bool useMask = false);
void ST7735_drawChar(ST7735_Handle *handle, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
void ST7735_getTextBounds(ST7735_Handle *handle, const char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
void ST7735_setTextSize(ST7735_Handle *handle, uint8_t sx, uint8_t sy);
void ST7735_setFont(ST7735_Handle *handle, const GFXfont *f = NULL);
void ST7735_setCursor(ST7735_Handle *handle, int16_t x, int16_t y);
void ST7735_setTextColor(ST7735_Handle *handle, uint16_t c);
void ST7735_setTextColorWithBg(ST7735_Handle *handle, uint16_t c, uint16_t bg);
void ST7735_setTextWrap(ST7735_Handle *handle, bool w);
void ST7735_cp437(ST7735_Handle *handle, bool x = true);
int16_t ST7735_width(ST7735_Handle *handle);
int16_t ST7735_height(ST7735_Handle *handle);
uint8_t ST7735_getRotation(ST7735_Handle *handle);
int16_t ST7735_getCursorX(ST7735_Handle *handle);
int16_t ST7735_getCursorY(ST7735_Handle *handle);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ADAFRUIT_ST7735_API_H