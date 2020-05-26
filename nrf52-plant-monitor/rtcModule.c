/*
 * rtcModule.c
 *
 *  Created on: May 4, 2020
 *      Author: seregia
 */

#include "nrf_drv_rtc.h"
#include "stdbool.h"
#include <nrf_delay.h>

#include "rtcModule.h"

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(0); /**< Declaring an instance of nrf_drv_rtc for RTC0. */

static volatile bool delayCompleted = true;

void rtc_handler(nrf_drv_rtc_int_type_t int_type) {
	if (int_type == NRF_DRV_RTC_INT_COMPARE0) {
		delayCompleted = true;
	}
}

void clocksStart() {
	// Start HFCLK and wait for it to start.
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 1;
	while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}

void rtcModule_init() {
	uint32_t err_code;

	clocksStart();

	nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
	config.prescaler = 32;
	err_code = nrf_drv_rtc_init(&rtc, &config, rtc_handler);
	APP_ERROR_CHECK(err_code);

	//Enable tick event & interrupt
	//nrf_drv_rtc_tick_enable(&rtc,false);

	err_code = nrf_drv_rtc_cc_set(&rtc, 0, 0, true);
	APP_ERROR_CHECK(err_code);

	//Power on RTC instance
	nrf_drv_rtc_enable(&rtc);
}

void rtcModule_interruptDelay() {
	nrfx_rtc_counter_clear(&rtc);
	nrf_drv_rtc_cc_set(&rtc, 0, 1, true);
}

void rtcModule_delayMsLowPower(uint32_t delayTimeMs) {
	nrfx_rtc_counter_clear(&rtc);
	nrf_drv_rtc_cc_set(&rtc, 0, delayTimeMs, true);

	delayCompleted = false;

	while (!delayCompleted) {
		NRF_POWER->TASKS_LOWPWR = 1;
		NRF_CLOCK->TASKS_HFCLKSTOP = 1;

		__SEV();
		__WFE();
		__WFE();
	}

	NRF_POWER->TASKS_LOWPWR = 0;
	clocksStart();
	NVIC_EnableIRQ(POWER_CLOCK_IRQn);
}
