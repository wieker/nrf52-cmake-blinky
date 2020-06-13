/*
 * saadcHelper.h
 *
 *  Created on: Jun 7, 2020
 *      Author: seregia
 */

#ifndef SAADCHELPER_H_
#define SAADCHELPER_H_

#include "nrf_drv_saadc.h"

#define SAADC_MAX_VALUE 4096

void saadcHelper_initSaadc(uint8_t channel, nrf_saadc_input_t input, nrf_saadc_resistor_t resistor, nrf_saadc_reference_t reference, nrf_saadc_gain_t gain);
void saadcHelper_shutdownSaadc();
int16_t saadcHelper_getSaadcValue(uint8_t channel);

#endif /* SAADCHELPER_H_ */
