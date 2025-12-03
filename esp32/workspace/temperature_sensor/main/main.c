#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gptimer.h"
#include "driver/gptimer_types.h"
#include "driver/temperature_sensor.h"
#include "esp_err.h"
#include "esp_log.h"
#include "hal/temperature_sensor_ll.h"

#define REG(REGISTER) ((volatile uint32_t *) (REGISTER))

void init_tsens(void);
void init_timer(void);
bool timer_alarm_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);

temperature_sensor_handle_t sensor = NULL;
temperature_sensor_config_t tsense_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(-10, 80);

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

bool timer_alarm_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
	float tsens_out;
	
	ESP_ERROR_CHECK(temperature_sensor_get_celsius(sensor, &tsens_out));
	printf("Current temperature: %.2f °C\n", tsens_out);
	
	return false;
}

void init_tsens(void) {
	ESP_ERROR_CHECK(temperature_sensor_install(&tsense_config, &sensor));
	ESP_ERROR_CHECK(temperature_sensor_enable(sensor));
}

void init_timer(void) {
	ESP_ERROR_CHECK(gptimer_new_timer(&config, &gptimer)); // Create a new timer instance
	ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config)); // Set the timer's alarm configuration
	ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &gptimer_cbs, NULL)); // Register an alarm event callback function

	//ESP_ERROR_CHECK(gptimer_enable(gptimer)); // Enable the timer
	//ESP_ERROR_CHECK(gptimer_start(gptimer)); // Start the timer
}

void app_main(void)
{
	init_tsens();
	init_timer();
	
    while(true){
		float tsens_out;
		
		uint32_t raw_temp_val = temperature_sensor_ll_get_raw_value();
	
		ESP_ERROR_CHECK(temperature_sensor_get_celsius(sensor, &tsens_out));
		ESP_LOGI("Temperature Sensor", "Current temperature: %f °C, Raw Value: %d\n", tsens_out, raw_temp_val);
		
		sleep(1);
	}
}
