/*
 * analogueButtonArray.c
 *
 *  Created on: Jun 7, 2020
 *      Author: seregia
 */

#include "batteryMeter.h"
#include "nrf_delay.h"
#include "boards.h"


#define SAADC_READS_COUNT 10

double batteryMeter_getVoltage() {

	saadcHelper_initSaadc(3, NRF_SAADC_INPUT_VDD, NRF_SAADC_RESISTOR_DISABLED, NRF_SAADC_REFERENCE_INTERNAL, NRF_SAADC_GAIN1_6);

	nrf_delay_ms(10);

	int32_t unVal = 0;
	for (int var = 0; var < SAADC_READS_COUNT; ++var) {
		unVal += saadcHelper_getSaadcValue(3);
		nrf_delay_ms(1);
	}

	unVal /= SAADC_READS_COUNT;

	saadcHelper_shutdownSaadc();

	return unVal / 3752.0 * 3.3;
}

