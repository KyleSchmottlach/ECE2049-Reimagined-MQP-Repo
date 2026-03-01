#include "Adafruit_ST7735_API.h"
#include "Adafruit_ST7735.h"

struct ST7735_Handle {
    Adafruit_ST7735 instance;
};

static ST7735_Handle st7735;