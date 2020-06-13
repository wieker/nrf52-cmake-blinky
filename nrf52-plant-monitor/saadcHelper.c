/*
 * saadcHelper.c
 *
 *  Created on: Jun 7, 2020
 *      Author: seregia
 */
#include "stdbool.h"
#include "saadcHelper.h"

void saadc_callback(nrf_drv_saadc_evt_t const * p_event) {}

void saadcHelper_initSaadc(uint8_t channel, nrf_saadc_input_t input, nrf_saadc_resistor_t resistor, nrf_saadc_reference_t reference, nrf_saadc_gain_t gain)
{
	ret_code_t err_code;

	nrf_saadc_channel_config_t channel_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(input);
	channel_config.reference = reference;
	channel_config.gain = gain;
	channel_config.acq_time = NRF_SAADC_ACQTIME_10US;
	channel_config.mode = NRF_SAADC_MODE_SINGLE_ENDED;
	channel_config.pin_p = input;
	channel_config.pin_n = NRF_SAADC_INPUT_DISABLED;
	channel_config.resistor_p = resistor;
	channel_config.resistor_n = NRF_SAADC_RESISTOR_DISABLED;

	nrf_drv_saadc_config_t saadc_config;
	saadc_config.low_power_mode = false;
	saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
	saadc_config.oversample = NRF_SAADC_OVERSAMPLE_DISABLED;
	saadc_config.interrupt_priority = APP_IRQ_PRIORITY_LOW;

	err_code = nrf_drv_saadc_init(&saadc_config, saadc_callback);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_drv_saadc_channel_init(channel, &channel_config);
	APP_ERROR_CHECK(err_code);
}

void saadcHelper_shutdownSaadc() {
	nrf_drv_saadc_abort();
	nrf_drv_saadc_uninit();
	while (nrf_drv_saadc_is_busy());
}

int16_t saadcHelper_getSaadcValue(uint8_t channel) {
	nrf_saadc_value_t adcValue;
	ret_code_t ret_code = nrf_drv_saadc_sample_convert(channel, &adcValue);
	APP_ERROR_CHECK(ret_code);

	return (int16_t)adcValue;
}
