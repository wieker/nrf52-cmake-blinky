/*
 * soilMoistureSensor.c
 *
 *  Created on: Apr 25, 2020
 *      Author: seregia
 */

#include "stdbool.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_delay.h"
#include "soilMoistureSensor.h"
#include "saadcHelper.h"

#define COUNT_PROBES 5
#define DELAY_US_PROBES 100

#define SAADC_CHANEL 0
#define SENSOR_MAX_VALUE 11024

void soilMoistureSensor_init() {
	nrf_gpio_cfg_output(SENS_PIN_OUT);
}

int16_t getDelta(bool bPositive) {
	int16_t nBefore = saadcHelper_getSaadcValue(SAADC_CHANEL);
	nrf_gpio_pin_write(SENS_PIN_OUT, bPositive);
	int16_t nAfter = saadcHelper_getSaadcValue(SAADC_CHANEL);

	return bPositive
			? nAfter - nBefore
			: nBefore - nAfter;
}

int16_t soilMoistureSensor_getSoilConductivity()
{
	saadcHelper_initSaadc(SAADC_CHANEL, NRF_SAADC_INPUT_AIN0, NRF_SAADC_RESISTOR_PULLDOWN, NRF_SAADC_REFERENCE_VDD4, NRF_SAADC_GAIN1_4);

	bool bHasLoad = true;
	int32_t nResult = 0;
	for (int nProbeNr = 0; nProbeNr < COUNT_PROBES && bHasLoad; nProbeNr++) {

		int16_t nPosDelta = getDelta(true);
		nrf_delay_us(DELAY_US_PROBES);
		int16_t nNegDelta = getDelta(false);
		nrf_delay_us(DELAY_US_PROBES);

		bHasLoad = nPosDelta > 0 && nNegDelta > 0;

		nResult = bHasLoad
				? nResult + nPosDelta + nNegDelta
				: 0;
	}

	saadcHelper_shutdownSaadc();
	nResult /= COUNT_PROBES;

	return (int16_t)(((double)nResult / SENSOR_MAX_VALUE) * 100);
}

