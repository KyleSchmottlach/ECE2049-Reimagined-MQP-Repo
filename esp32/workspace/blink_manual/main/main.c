#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_err.h"
#include "esp_log.h"
#include "hal/gpio_types.h"
#include "hal/timer_types.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"

#define BLINK_GPIO GPIO_NUM_0

// Function stubs
void configure_led_gpio(void);
void configure_timer(void);
bool timer_alarm_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);

void switch_led();

// State variable storing LED state
static uint32_t led_state = 0;

// Timer configuration variables
gptimer_handle_t gptimer = NULL;
gptimer_config_t const config = {
	.clk_src = GPTIMER_CLK_SRC_DEFAULT, // Sets the timer to the default system clock source
	.direction = GPTIMER_COUNT_UP, // Sets the timer to count up mode
	.resolution_hz = 1 * 1000 * 1000 // Sets the timer to use a resolution of 1 MHz (1_000_000 Hz)
};
gptimer_alarm_config_t const alarm_config = {
	.alarm_count = 500000, // Sets the timer to trigger an alarm event at a count of 1_000_000 (or 1 second since the timer counts in 1 MHz)
	.reload_count = 0, // Sets the timer to reload the alarm value back to 0 if the auto_reload_on_alarm flag is set true
	.flags.auto_reload_on_alarm = true // Ensures alarm reloading occurs
};
/**
* Interesting note about ESP32: many peripherals use a system called the Event Task Matrix to directly notify each other of particular events 
* without using CPU interrupts. Lowers task latency and CPU usage, since peripherals can handle their own events
*/
gptimer_event_callbacks_t const gptimer_cbs = {
	.on_alarm = timer_alarm_callback // Sets the callback for the alarm event
};

/**
* Helper function to configure the GPIO pin for the output LED
*/
void configure_led_gpio(void) {
	gpio_reset_pin(BLINK_GPIO); // Resets the settings for the GPIO pin. Necessary since the boot process can change the pin settings
										  // especially on special "strapping" pins
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT); // Set the GPIO mode to output
	gpio_set_pull_mode(BLINK_GPIO, GPIO_FLOATING); // Ensure that the pull mode is set to floating
}

/**
* Helper function to configure the settings for the general purpose timer and its alarm events
*/
void configure_timer(void) {
	ESP_ERROR_CHECK(gptimer_new_timer(&config, &gptimer)); // Create a new timer instance
	ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config)); // Set the timer's alarm configuration
	ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &gptimer_cbs, NULL)); // Register an alarm event callback function

	ESP_ERROR_CHECK(gptimer_enable(gptimer)); // Enable the timer
	ESP_ERROR_CHECK(gptimer_start(gptimer)); // Start the timer
}

/**
* General Purpose timer alarm callback function.
*
* Parameters:
* - timer: Pointer to the timer that created the event
* - edata: Pointer to data structure containing important event data, such as the alarm count and the timer's count value
* - user_ctx: arbitrary context data that the user can pass in to the function. Can be anything
*/
bool timer_alarm_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
	switch_led(); // Switch the LED output every alarm

	return false;
}

/**
* Function that switches the LED state via the GPIO pins
*/
void switch_led() {
	ESP_ERROR_CHECK(gpio_set_level(BLINK_GPIO, led_state)); // Sets the level of the GPIO pin to either high or low

	led_state = !led_state; // Flips the led state
}

void app_main(void)
{
	// Configure the led and the timer
	configure_led_gpio();
	configure_timer();

	// Perform a loop 
    while (true) {
        sleep(UINT32_MAX); // Sleep the core for the maximum amount of time possible. Alarm events mean the CPU essentially does
									// no work on its own once they are running
    }
}
