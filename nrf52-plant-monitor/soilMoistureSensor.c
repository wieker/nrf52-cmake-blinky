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

#include "nrf_drv_saadc.h"

#define SENS_PIN_OUT 6
#define SENS_PIN_IN 2

#define COUNT_PROBES 3
#define DELAY_US_PROBES 100

void saadc_callback(nrf_drv_saadc_evt_t const * p_event) {}

void initSaadc(void)
{
	ret_code_t err_code;

	nrf_saadc_channel_config_t channel_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);
	channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL;
	channel_config.gain = NRF_SAADC_GAIN1_5;
	channel_config.acq_time = NRF_SAADC_ACQTIME_10US;
	channel_config.mode = NRF_SAADC_MODE_SINGLE_ENDED;
	channel_config.pin_p = NRF_SAADC_INPUT_AIN0;
	channel_config.pin_n = NRF_SAADC_INPUT_DISABLED;
	channel_config.resistor_p = NRF_SAADC_RESISTOR_PULLDOWN;
	channel_config.resistor_n = NRF_SAADC_RESISTOR_DISABLED;

	nrf_drv_saadc_config_t saadc_config;
	saadc_config.low_power_mode = false;
	saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
	saadc_config.oversample = NRF_SAADC_OVERSAMPLE_DISABLED;
	saadc_config.interrupt_priority = APP_IRQ_PRIORITY_LOW;

	err_code = nrf_drv_saadc_init(&saadc_config, saadc_callback);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_drv_saadc_channel_init(0, &channel_config);
	APP_ERROR_CHECK(err_code);
}

void shutdownSaadc(void) {
	nrf_drv_saadc_abort();
	nrf_drv_saadc_uninit();
	while (nrf_drv_saadc_is_busy());
}

int16_t getSaadcValue(void) {
	nrf_saadc_value_t adcValue;
	ret_code_t ret_code = nrf_drv_saadc_sample_convert(0, &adcValue);
	APP_ERROR_CHECK(ret_code);

	return (int16_t)adcValue;
}

void soilMoistureSensor_init() {
	nrf_gpio_cfg_output(SENS_PIN_OUT);
	nrf_gpio_cfg_input(SENS_PIN_IN, NRF_GPIO_PIN_NOPULL);
}

int16_t getDelta(bool bPositive) {
	int16_t nBefore = getSaadcValue();
	nrf_gpio_pin_write(SENS_PIN_OUT, bPositive);
	int16_t nAfter = getSaadcValue();

	return bPositive
			? nAfter - nBefore
			: nBefore - nAfter;
}

int16_t soilMoistureSensor_getSoilConductivity()
{
	initSaadc();

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

	shutdownSaadc();

	return (int16_t)(nResult /= COUNT_PROBES);
}

