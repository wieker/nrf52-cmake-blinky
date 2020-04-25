/*
 * soilMoistureSensor.c
 *
 *  Created on: Apr 25, 2020
 *      Author: seregia
 */

#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_delay.h"
#include "soilMoistureSensor.h"

#define SENS_PIN_OUT 3
#define SENS_PIN_IN 2

#define COUNT_PROBES 10
#define DELAY_PROBES 10


void soilMoistureSensor_setup() {
	nrf_gpio_cfg_output(SENS_PIN_OUT);
	nrf_gpio_cfg_input(SENS_PIN_IN, NRF_GPIO_PIN_NOPULL);
}

BOOL soilMoistureSensor_hasWater()
{
    BOOL bHasLoad = TRUE;
    for (int nProbeNr = 0; nProbeNr < COUNT_PROBES && bHasLoad; nProbeNr++)
    {
    	nrf_gpio_pin_write(SENS_PIN_OUT, HIGH);
    	nrf_delay_ms(DELAY_PROBES);
        bHasLoad = bHasLoad && nrf_gpio_pin_read(SENS_PIN_IN);

        nrf_gpio_pin_write(SENS_PIN_OUT, LOW);
        nrf_delay_ms(DELAY_PROBES);
        bHasLoad = bHasLoad && !nrf_gpio_pin_read(SENS_PIN_IN);
    }

    return bHasLoad;
}

