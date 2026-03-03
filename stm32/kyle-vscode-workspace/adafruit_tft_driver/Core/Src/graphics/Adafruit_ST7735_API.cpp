#include "Adafruit_ST7735_API.h"
#include "Adafruit_ST7735.h"

struct ST7735_Handle {
    Adafruit_ST7735 instance;
};



//--------------------------------------------------------------------------------------------
// Adafruit_ST7735.cpp function definitions

ST7735_Handle* ST7735_create(SPI_HandleTypeDef *spiHandle, int16_t cs, GPIO_TypeDef *cs_port, int16_t dc, GPIO_TypeDef *dc_port, volatile bool *spiTxDone) {
    static ST7735_Handle st7735 = ST7735_Handle{Adafruit_ST7735(spiHandle, cs, cs_port, dc, dc_port, -1, spiTxDone)};
    return &st7735;
}

void ST7735_init(ST7735_Handle* handle, uint8_t options) {
    handle->instance.initR(options);
}

void ST7735_setRotation(ST7735_Handle* handle, uint8_t m) {
    handle->instance.setRotation(m);
}

//--------------------------------------------------------------------------------------------
// Adafruit_ST77xx.cpp function definitions

void ST7735_setAddrWindow(ST7735_Handle *handle, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    handle->instance.setAddrWindow(x, y, w, h);
}

void ST7735_enableDisplay(ST7735_Handle *handle, bool enable) {
    handle->instance.enableDisplay(enable);
}

void ST7735_enableTearing(ST7735_Handle *handle, bool enable) {
    handle->instance.enableTearing(enable);
}

void ST7735_enableSleep(ST7735_Handle *handle, bool enable) {
    handle->instance.enableSleep(enable);
}

//--------------------------------------------------------------------------------------------
// Adafruit_SPITFT.cpp function definitions

void ST7735_drawPixel(ST7735_Handle *handle, int16_t x, int16_t y, uint16_t color) {
    handle->instance.drawPixel(x, y, color);
}

void ST7735_fillRect(ST7735_Handle *handle, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    handle->instance.fillRect(x, y, w, h, color);
}

void ST7735_drawFastHLine(ST7735_Handle *handle, int16_t x, int16_t y, int16_t w, uint16_t color) {
    handle->instance.drawFastHLine(x, y, w, color);
}

void ST7735_drawFastVLine(ST7735_Handle *handle, int16_t x, int16_t y, int16_t h, uint16_t color) {
    handle->instance.drawFastVLine(x, y, h, color);
}

void ST7735_invertDisplay(ST7735_Handle *handle, bool i) {
    handle->instance.invertDisplay(i);
}

uint16_t ST7735_color565(ST7735_Handle *handle, uint8_t r, uint8_t g, uint8_t b) {
    return handle->instance.color565(r, g, b);
}

void ST7735_drawRGBBitmap(ST7735_Handle *handle, int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h) {
    handle->instance.drawRGBBitmap(x, y, pcolors, w, h);
}
void ST7735_drawRGBBitmapMask(ST7735_Handle *handle, int16_t x, int16_t y, uint16_t *pcolors, int16_t w, int16_t h, uint8_t *mask) {
    handle->instance.drawRGBBitmap(x, y, pcolors, mask, w, h);
}

//--------------------------------------------------------------------------------------------
// Adafruit_GFX.cpp function definitions
void ST7735_fillScreen(ST7735_Handle *handle, uint16_t color) {
    handle->instance.fillScreen(color);
}
void ST7735_drawLine(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color){
    handle->instance.drawLine(x0, y0, x1, y1, color);
}
void ST7735_drawRect(ST7735_Handle *handle, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
    handle->instance.drawRect(x, y, w, h, color);
}
void ST7735_drawCircle(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t r, uint16_t color){
    handle->instance.drawCircle(x0, y0, r, color);
}
void ST7735_drawCircleHelper(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color){
    handle->instance.drawCircleHelper(x0, y0, r, cornername, color);
}
void ST7735_fillCircle(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t r, uint16_t color){
    handle->instance.fillCircle(x0, y0, r, color);
}
void ST7735_fillCircleHelper(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color){
    handle->instance.fillCircleHelper(x0, y0, r, cornername, delta, color);
}
void ST7735_drawEllipse(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t rw, int16_t rh, uint16_t color){
    handle->instance.drawEllipse(x0, y0, rw, rh, color);
}
void ST7735_fillEllipse(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t rw, int16_t rh, uint16_t color){
    handle->instance.fillEllipse(x0, y0, rw, rh, color);
}
void ST7735_drawTriangle(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){
    handle->instance.drawTriangle(x0, y0, x1, y1, x2, y2, color);
}
void ST7735_fillTriangle(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){
    handle->instance.fillTriangle(x0, y0, x1, y1, x2, y2, color);
}
void ST7735_drawRoundRect(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color){
    handle->instance.drawRoundRect(x0, y0, w, h, radius, color);
}
void ST7735_fillRoundRect(ST7735_Handle *handle, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color){
    handle->instance.fillRoundRect(x0, y0, w, h, radius, color);
}
void ST7735_drawBitmap(ST7735_Handle *handle, int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color){
    handle->instance.drawBitmap(x, y, bitmap, w, h, color);
}
void ST7735_drawBitmapBg(ST7735_Handle *handle, int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg){
    handle->instance.drawBitmap(x, y, bitmap, w, h, color, bg);
}
void ST7735_drawXBitmap(ST7735_Handle *handle, int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color){
    handle->instance.drawXBitmap(x, y, bitmap, w, h, color);
}
void ST7735_drawGrayscaleBitmap(ST7735_Handle *handle, int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h){
    handle->instance.drawGrayscaleBitmap(x, y, bitmap, w, h);
}
void ST7735_drawGrayscaleBitmapMask(ST7735_Handle *handle, int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint8_t *mask){
    handle->instance.drawGrayscaleBitmap(x, y, bitmap, mask, w, h);
}
void ST7735_drawChar(ST7735_Handle *handle, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y){
    handle->instance.drawChar(x, y, c, color, bg, size_x, size_y);
}
void ST7735_getTextBounds(ST7735_Handle *handle, const char *string, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h){
    handle->instance.getTextBounds(string, x, y, x1, y1, w, h);
}
void ST7735_setTextSize(ST7735_Handle *handle, uint8_t sx, uint8_t sy){
    handle->instance.setTextSize(sx, sy);
}
void ST7735_setFont(ST7735_Handle *handle, const GFXfont *f){
    handle->instance.setFont(f);
}
void ST7735_setCursor(ST7735_Handle *handle, int16_t x, int16_t y){
    handle->instance.setCursor(x, y);
}
void ST7735_setTextColor(ST7735_Handle *handle, uint16_t c){
    handle->instance.setTextColor(c);
}
void ST7735_setTextColorWithBg(ST7735_Handle *handle, uint16_t c, uint16_t bg){
    handle->instance.setTextColor(c, bg);
}
void ST7735_setTextWrap(ST7735_Handle *handle, bool w){
    handle->instance.setTextWrap(w);
}
void ST7735_cp437(ST7735_Handle *handle, bool x){
    handle->instance.cp437(x);
}
int16_t ST7735_width(ST7735_Handle *handle){
    return handle->instance.width();
}
int16_t ST7735_height(ST7735_Handle *handle){
    return handle->instance.height();
}
uint8_t ST7735_getRotation(ST7735_Handle *handle){
    return handle->instance.getRotation();
}
int16_t ST7735_getCursorX(ST7735_Handle *handle){
    return handle->instance.getCursorX();
}
int16_t ST7735_getCursorY(ST7735_Handle *handle){
    return handle->instance.getCursorY();
}