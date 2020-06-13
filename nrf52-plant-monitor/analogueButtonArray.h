/*
 * analogueButtonArray.h
 *
 *  Created on: Jun 7, 2020
 *      Author: seregia
 */

#ifndef ANALOGUEBUTTONARRAY_H_
#define ANALOGUEBUTTONARRAY_H_

#include <stdint.h>
#include "saadcHelper.h"

int8_t analogueButtonArray_get(uint32_t pin_number, uint8_t channel, nrf_saadc_input_t input);

#endif /* ANALOGUEBUTTONARRAY_H_ */
