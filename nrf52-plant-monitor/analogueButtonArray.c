/*
 * analogueButtonArray.c
 *
 *  Created on: Jun 7, 2020
 *      Author: seregia
 */

#include "analogueButtonArray.h"
#include "nrf_delay.h"
#include "boards.h"


double pullDownResistors[] = { 10020, 19995, 39230, 82350 };
#define PULLUP_RESISTOR_OHMS 14120
#define REFERENCE_VOLTAGE 3.3
#define CALIBRATED_REF_VOLTAGE 3.1
#define RESISTORS_COUNT (sizeof(pullDownResistors) / sizeof(pullDownResistors[0]))
#define LAST_RESISTOR_IDX (RESISTORS_COUNT - 1)

#define SAADC_READS_COUNT 10

double substractResistor(double commonImpedance, double resistor) {
	return resistor * commonImpedance / (resistor - commonImpedance);
}

uint8_t getBitMask(double commonImpedance) {
	uint8_t nBitMask = 0;
	for (uint8_t i = 0; i < RESISTORS_COUNT && commonImpedance < pullDownResistors[LAST_RESISTOR_IDX] * 2.0 && commonImpedance >= 0; i++) {
		double resistor = pullDownResistors[i];
		if (commonImpedance < substractResistor(resistor, pullDownResistors[3] * 2)) {
			nBitMask |= 1 << (LAST_RESISTOR_IDX - i);
			commonImpedance = substractResistor(commonImpedance, resistor);
		}
	}

	return nBitMask;
}

double getSwitchImpedance(double voltage) {
	return PULLUP_RESISTOR_OHMS * voltage / (REFERENCE_VOLTAGE - voltage);
}

int8_t analogueButtonArray_get(uint32_t pin_number, uint8_t channel, nrf_saadc_input_t input) {

	nrf_gpio_cfg_input(pin_number, NRF_GPIO_PIN_PULLUP);
	saadcHelper_initSaadc(channel, input, NRF_SAADC_RESISTOR_PULLUP, NRF_SAADC_REFERENCE_VDD4, NRF_SAADC_GAIN1_4);

	nrf_delay_ms(10);

	int32_t unVal = 0;
	for (int var = 0; var < SAADC_READS_COUNT; ++var) {
		unVal += saadcHelper_getSaadcValue(channel);
		nrf_delay_ms(1);
	}

	unVal /= SAADC_READS_COUNT;

	saadcHelper_shutdownSaadc();

	double voltage = (double)unVal / SAADC_MAX_VALUE * CALIBRATED_REF_VOLTAGE;
	double commonSwitchImpedance = getSwitchImpedance(voltage);
	uint8_t bitMask = getBitMask(commonSwitchImpedance);

	return bitMask;
}
