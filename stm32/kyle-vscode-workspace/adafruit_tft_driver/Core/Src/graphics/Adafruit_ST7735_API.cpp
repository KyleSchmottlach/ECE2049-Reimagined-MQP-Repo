#include "Adafruit_ST7735_API.h"

struct ST7735_Handle {
    Adafruit_ST7735 instance;
};

//--------------------------------------------------------------------------------------------
// Adafruit_ST7735.cpp function definitions

ST7735_Handle* ST7735_create(SPI_HandleTypeDef *spiHandle, int16_t cs, GPIO_TypeDef *cs_port, int16_t dc, GPIO_TypeDef *dc_port, int8_t rst) {
    static ST7735_Handle st7735 = ST7735_Handle{Adafruit_ST7735(spiHandle, cs, cs_port, dc, dc_port, rst)};
    return &st7735;
}

void ST7735_init(ST7735_Handle* handle, uint8_t options) {
    handle->instance.initR(options);
}

void setRotation(ST7735_Handle* handle, uint8_t m) {
    handle->instance.setRotation(m);
}

//--------------------------------------------------------------------------------------------
// Adafruit_ST77xx.cpp function definitions

void setAddrWindow(ST7735_Handle *handle, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    handle->instance.setAddrWindow(x, y, w, h);
}

void enableDisplay(ST7735_Handle *handle, bool enable) {
    handle->instance.enableDisplay(enable);
}

void enableTearing(ST7735_Handle *handle, bool enable) {
    handle->instance.enableTearing(enable);
}

void enableSleep(ST7735_Handle *handle, bool enable) {
    handle->instance.enableSleep(enable);
}

//--------------------------------------------------------------------------------------------
// Adafruit_SPITFT.cpp function definitions

void drawPixel(ST7735_Handle *handle, int16_t x, int16_t y, uint16_t color) {
    handle->instance.drawPixel(x, y, color);
}

void fillRect(ST7735_Handle *handle, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    handle->instance.fillRect(x, y, w, h, color);
}

void drawFastHLine(ST7735_Handle *handle, int16_t x, int16_t y, int16_t w, uint16_t color) {
    handle->instance.drawFastHLine(x, y, w, color);
}

void drawFastVLine(ST7735_Handle *handle, int16_t x, int16_t y, int16_t h, uint16_t color) {
    handle->instance.drawFastVLine(x, y, h, color);
}

void invertDisplay(ST7735_Handle *handle, bool i) {
    handle->instance.invertDisplay(i);
}

uint16_t color565(ST7735_Handle *handle, uint8_t r, uint8_t g, uint8_t b) {
    return handle->instance.color565(r, g, b);
}

void drawRGBBitmap(ST7735_Handle *handle, int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h) {
    handle->instance.drawRGBBitmap(x, y, pcolors, w, h);
}