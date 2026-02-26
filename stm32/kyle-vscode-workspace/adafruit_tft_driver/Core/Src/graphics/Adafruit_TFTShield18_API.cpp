#include "Adafruit_TFTShield18_API.h"
#include "Adafruit_TFTShield18.h"

/*!
* Wrapper class for the Adafruit TFT Shield 1.8" that provides a C API for the C++ class defined in 
* Adafruit_TFTShield18.h. This is used to allow the TFT shield to be used in C code
*/

struct TFTShield18_Handle {
    Adafruit_TFTShield18 instance;
};

static TFTShield18_Handle seesaw;

TFTShield18_Handle* TFTShield18_create() {
    seesaw = TFTShield18_Handle{Adafruit_TFTShield18()};
    return &seesaw;
}

bool TFTShield18_begin(TFTShield18_Handle* handle, uint8_t addr) {
    return handle->instance.begin(addr);
}

void TFTShield18_setBacklight(TFTShield18_Handle* handle, uint16_t value) {
    handle->instance.setBacklight(value);
}

void TFTShield18_setBacklightFreq(TFTShield18_Handle* handle, uint16_t freq) {
    handle->instance.setBacklightFreq(freq);
}
void TFTShield18_tftReset(TFTShield18_Handle* handle, bool rst) {
    handle->instance.tftReset(rst);
}
uint32_t TFTShield18_readButtons(TFTShield18_Handle* handle) {
    return handle->instance.readButtons();
}
