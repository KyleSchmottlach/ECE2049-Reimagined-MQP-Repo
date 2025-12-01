#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "hal/gpio_types.h"
#include "hal/timer_types.h"
#include "soc/clk_tree_defs.h"
#include "soc/gpio_num.h"
#include "soc/gpio_reg.h"
#include "soc/io_mux_reg.h"

#define REG(REGISTER) ((volatile uint32_t *) (REGISTER))

#define BLUE_LED GPIO_NUM_8
#define RED_LED GPIO_NUM_1
#define YELLOW_LED GPIO_NUM_0
#define GREEN_LED GPIO_NUM_7

#define BLUE_BUTTON GPIO_NUM_10
#define RED_BUTTON GPIO_NUM_11
#define YELLOW_BUTTON GPIO_NUM_2
#define GREEN_BUTTON GPIO_NUM_3

#define DEBOUNCE_DELAY_MS 100


// Function stubs
void configure_led_gpio_hal(void);
void configure_led_gpio_bare(void);
void configure_timer(void);
bool timer_alarm_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);

void reset_gpio_bare(uint8_t gpio_pin);
void config_gpio_output(uint8_t gpio_pin);
void config_gpio_input(uint8_t gpio_pin, bool pullup);

void switch_led_hal();
void switch_led_bare();

bool read_button(uint8_t *currReading, uint64_t *lastDebounceTime, uint8_t *buttonState);

uint32_t get_io_mux_from_gpio(uint8_t gpio_pin);

// State variable storing LED state
static uint32_t led_state = 0;

// Debounce values
uint64_t lastBlueDebounceTime = 0;
uint64_t lastRedDebounceTime = 0;
uint64_t lastYellowDebounceTime = 0;
uint64_t lastGreenDebounceTime = 0;

uint8_t blueButtonState = 1;
uint8_t redButtonState = 1;
uint8_t yellowButtonState = 1;
uint8_t greenButtonState = 1;

uint8_t lastBlueState = 1;
uint8_t lastRedState = 1;
uint8_t lastYellowState = 1;
uint8_t lastGreenState = 1;

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

uint32_t get_io_mux_from_gpio(uint8_t gpio_pin) {
	switch(gpio_pin){
	case BLUE_LED:
		return IO_MUX_GPIO8_REG;
	case RED_LED:
		return IO_MUX_GPIO1_REG;
	case YELLOW_LED:
		return IO_MUX_GPIO0_REG;
	case GREEN_LED:
		return IO_MUX_GPIO7_REG;
	case BLUE_BUTTON:
		return IO_MUX_GPIO10_REG;
	case RED_BUTTON:
		return IO_MUX_GPIO11_REG;
	case YELLOW_BUTTON:
		return IO_MUX_GPIO2_REG;
	case GREEN_BUTTON:
		return IO_MUX_GPIO3_REG;
	default:
		return 0;
	}
}

/**
* Helper function to configure the GPIO pin for the output LED
*/
void configure_led_gpio_hal(void) {
	gpio_reset_pin(BLUE_LED); // Resets the settings for the GPIO pin. Necessary since the boot process can change the pin settings
										  // especially on special "strapping" pins
	gpio_set_direction(BLUE_LED, GPIO_MODE_OUTPUT); // Set the GPIO mode to output
	gpio_set_pull_mode(BLUE_LED, GPIO_FLOATING); // Ensure that the pull mode is set to floating
}

void reset_gpio_bare(uint8_t gpio_pin) {
	uint32_t io_mux_register = get_io_mux_from_gpio(gpio_pin);	

	PIN_INPUT_DISABLE(io_mux_register);
	PIN_PULLDWN_DIS(io_mux_register);
	PIN_PULLUP_DIS(io_mux_register);
}

void config_gpio_output(uint8_t gpio_pin) {
	PIN_FUNC_SELECT(get_io_mux_from_gpio(gpio_pin), FUNC_GPIO8_GPIO8);

	*REG(GPIO_FUNC0_OUT_SEL_CFG_REG + 4*gpio_pin) |= 128 & GPIO_FUNC0_OUT_SEL;
	*REG(GPIO_FUNC0_OUT_SEL_CFG_REG + 4*gpio_pin) |= GPIO_FUNC0_OEN_SEL;
	*REG(GPIO_ENABLE_W1TS_REG) = (1 << gpio_pin);
}

void config_gpio_input(uint8_t gpio_pin, bool pullup) {
	*REG(GPIO_FUNC0_IN_SEL_CFG_REG + 4*gpio_pin) |= GPIO_SIG0_IN_SEL;
	*REG(GPIO_FUNC0_IN_SEL_CFG_REG + 4*gpio_pin) |= (1U << gpio_pin) & GPIO_FUNC0_IN_SEL;

	uint32_t io_mux_reg = get_io_mux_from_gpio(gpio_pin);

	PIN_INPUT_ENABLE(io_mux_reg);

	if(pullup) {
		PIN_PULLUP_EN(io_mux_reg);
		PIN_PULLDWN_DIS(io_mux_reg);
	} else {
		PIN_PULLDWN_EN(io_mux_reg);
		PIN_PULLUP_DIS(io_mux_reg);
	}
}

void configure_led_gpio_bare(void) {
	// Helper macros to disable input mode on the GPIO pin, and the pulldown and pullup resistors
	reset_gpio_bare(BLUE_LED);
	reset_gpio_bare(RED_LED);
	reset_gpio_bare(YELLOW_LED);
	reset_gpio_bare(GREEN_LED);
	reset_gpio_bare(BLUE_BUTTON);
	reset_gpio_bare(RED_BUTTON);
	reset_gpio_bare(YELLOW_BUTTON);
	reset_gpio_bare(GREEN_BUTTON);
	

	// Direct register setting of output mode via the GPIO_OUT_REG
	// *gpio_func8_out_sel_cfg_reg |= 128 & GPIO_FUNC8_OUT_SEL;
	// *gpio_func8_out_sel_cfg_reg |= GPIO_FUNC8_OEN_SEL;
	// *gpio_enable_w1ts_reg = (1 << BLUE_LED);

	config_gpio_output(BLUE_LED);
	config_gpio_output(RED_LED);
	config_gpio_output(YELLOW_LED);
	config_gpio_output(GREEN_LED);

	config_gpio_input(BLUE_BUTTON, true);
	config_gpio_input(RED_BUTTON, true);
	config_gpio_input(YELLOW_BUTTON, true);
	config_gpio_input(GREEN_BUTTON, true);
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
	// switch_led_hal(); // Switch the LED output every alarm
	// switch_led_bare();

	return false;
}

/**
* Function that switches the LED state via the GPIO pins
*/
void switch_led_hal() {
	ESP_ERROR_CHECK(gpio_set_level(BLUE_LED, led_state)); // Sets the level of the GPIO pin to either high or low

	led_state = !led_state; // Flips the led state
}

void set_gpio_output_val(uint8_t gpio_pin, bool state) {
	if(state) *REG(GPIO_OUT_W1TS_REG) |= (1U << gpio_pin);
	else *REG(GPIO_OUT_W1TC_REG) |= (1U << gpio_pin);
}

void switch_led_bare() {
	if(led_state) {
		*REG(GPIO_OUT_W1TS_REG) |= (1U << BLUE_LED) | (1U << RED_LED) | (1U << YELLOW_LED) | (1U << GREEN_LED);
	} else {
		*REG(GPIO_OUT_W1TC_REG)  |= (1U << BLUE_LED) | (1U << RED_LED) | (1U << YELLOW_LED) | (1U << GREEN_LED);
	}

	led_state = !led_state;
}

bool read_button(uint8_t *currReading, uint64_t *lastDebounceTime, uint8_t *buttonState) {
	bool debouncedState = true;

	if(*currReading != *buttonState) {
		*buttonState = *currReading;
		*lastDebounceTime = esp_timer_get_time();
	} else if((esp_timer_get_time() - *lastDebounceTime) / 1000 > DEBOUNCE_DELAY_MS) {
		debouncedState = !!(*buttonState);
	}

	return debouncedState;
}

void app_main(void)
{
	// Configure the led and the timer
	// configure_led_gpio_hal();
	configure_led_gpio_bare();
	configure_timer();

	// Perform a loop 
    while (true) {
        // sleep(UINT32_MAX); // Sleep the core for the maximum amount of time possible. Alarm events mean the CPU essentially does
									// no work on its own once they are running

		uint32_t input = *REG(GPIO_IN_REG);

		uint8_t blueVal = ((input >> BLUE_BUTTON) & 1U);
		uint8_t redVal = ((input >> RED_BUTTON) & 1U);
		uint8_t yellowVal = ((input >> YELLOW_BUTTON) & 1U);
		uint8_t greenVal = ((input >> GREEN_BUTTON) & 1U);

		uint8_t blueButtonPressed = read_button(&blueVal, &lastBlueDebounceTime, &blueButtonState);
		uint8_t redButtonPressed = read_button(&redVal, &lastRedDebounceTime, &redButtonState);
		uint8_t yellowButtonPressed = read_button(&yellowVal, &lastYellowDebounceTime, &yellowButtonState);
		uint8_t greenButtonPressed = read_button(&greenVal, &lastGreenDebounceTime, &greenButtonState);

		set_gpio_output_val(BLUE_LED, !blueButtonPressed);
		set_gpio_output_val(RED_LED, !redButtonPressed);
		set_gpio_output_val(YELLOW_LED, !yellowButtonPressed);
		set_gpio_output_val(GREEN_LED, !greenButtonPressed);

		if(!blueButtonPressed && blueButtonPressed != lastBlueState) printf("Blue button pressed!\n");
		if(!redButtonPressed && redButtonPressed != lastRedState) printf("Red button pressed!\n");
		if(!yellowButtonPressed && yellowButtonPressed != lastYellowState) printf("Yellow button pressed!\n");
		if(!greenButtonPressed && greenButtonPressed != lastGreenState) printf("Green button pressed!\n");

		lastBlueState = blueButtonPressed;
		lastRedState = redButtonPressed;
		lastYellowState = yellowButtonPressed;
		lastGreenState = greenButtonPressed;
	}
}
