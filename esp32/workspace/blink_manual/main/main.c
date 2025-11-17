#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gpio.h"

void configure_led_gpio(void);

void configure_led_gpio(void) {
	
}

void app_main(void)
{
    while (true) {
        printf("Hello from app_main!\n");
        sleep(1);
    }
}
