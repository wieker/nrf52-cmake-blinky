/*
 * wakeButton.c
 *
 *  Created on: May 5, 2020
 *      Author: seregia
 */

#include "wakeButton.h"

#include "boards.h"

#include "nrf_drv_gpiote.h"
#include "rtcModule.h"
#include "nrf.h"

void in_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
	NVIC_SystemReset();
}

void wakeButton_init() {
	uint32_t err_code = nrf_drv_gpiote_init();
	APP_ERROR_CHECK(err_code);

	nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
	in_config.sense = NRF_GPIOTE_POLARITY_HITOLO;
	in_config.pull = NRF_GPIO_PIN_PULLUP;

	err_code = nrf_drv_gpiote_in_init(BUTTON_1, &in_config, in_pin_handler);
	APP_ERROR_CHECK(err_code);

	nrf_drv_gpiote_in_event_enable(BUTTON_1, true);
}
