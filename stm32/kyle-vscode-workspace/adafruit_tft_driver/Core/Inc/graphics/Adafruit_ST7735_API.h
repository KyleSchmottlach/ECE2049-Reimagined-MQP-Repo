#ifndef ADAFRUIT_ST7735_API_H
#define ADAFRUIT_ST7735_API_H

#include "stdint.h"
#include "stm32h5xx_hal.h"
#include "stdbool.h"

#include "Adafruit_ST7735_Macros.h"
#include "Adafruit_ST77xx_Macros.h"
#include "gfxfont.h"
#include "stm32h5xx_hal_spi.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct ST7735_Handle ST7735_Handle;

//---------------------------------------------------------------------------------
// Adafruit_ST7735.cpp function declarations

void Display_create(SPI_HandleTypeDef *spi_handle, int16_t cs, GPIO_TypeDef *cs_port, int16_t dc, GPIO_TypeDef *dc_port, volatile bool *spiTxDone);

void Display_init(uint8_t options);
void Display_setRotation(uint8_t m);
//---------------------------------------------------------------------------------
// Adafruit_ST77xx.cpp function declarations

void Display_setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void Display_enableDisplay(bool enable);
void Display_enableTearing(bool enable);
void Display_enableSleep(bool enable);

//---------------------------------------------------------------------------------
// Adafruit_SPITFT.cpp function declarations

void Display_drawPixel(int16_t x, int16_t y, uint16_t color);
void Display_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void Display_drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void Display_drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void Display_invertDisplay(bool i);
uint16_t Display_color565(uint8_t r, uint8_t g, uint8_t b);
void Display_drawRGBBitmap(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h);
void Display_drawRGBBitmapMask(int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h, uint8_t *mask, bool useMask);

//---------------------------------------------------------------------------------
// Adafruit_GFX.cpp function declarations
void Display_fillScreen(uint16_t color);
void Display_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void Display_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

void Display_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void Display_drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void Display_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void Display_fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void Display_drawEllipse(int16_t x0, int16_t y0, int16_t rw, int16_t rh, uint16_t color);
void Display_fillEllipse(int16_t x0, int16_t y0, int16_t rw, int16_t rh, uint16_t color);
void Display_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void Display_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void Display_drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
void Display_fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
void Display_drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void Display_drawBitmapBg(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg);
void Display_drawXBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
void Display_drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h);
void Display_drawGrayscaleBitmapMask(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint8_t *mask);
void Display_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
void Display_getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
void Display_setTextSize(uint8_t sx, uint8_t sy);
void Display_setFont(const GFXfont *f);
void Display_setCursor(int16_t x, int16_t y);
void Display_setTextColor(uint16_t c);
void Display_setTextColorWithBg(uint16_t c, uint16_t bg);
void Display_setTextWrap(bool w);
void Display_cp437(bool x);
int16_t Display_width();
int16_t Display_height();
uint8_t Display_getRotation();
int16_t Display_getCursorX();
int16_t Display_getCursorY();
void Display_print(const char *str);
void Display_println(const char *str);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ADAFRUIT_ST7735_API_H