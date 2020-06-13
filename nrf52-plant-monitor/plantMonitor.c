
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "sdk_common.h"
#include "nrf.h"
#include "nrf_error.h"

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "app_util.h"
#include <nrf_delay.h>

#include "nrf_drv_clock.h"
#include "nrf_soc.h"

#include "soilMoistureSensor.h"
#include "esbTx.h"
#include "rtcModule.h"
#include "buzzer.h"
#include "wakeButton.h"
#include "analogueButtonArray.h"
#include "batteryMeter.h"

#define BSP_BOARD_LED_GREEN BSP_BOARD_LED_2
#define BSP_BOARD_LED_BLUE BSP_BOARD_LED_1
#define BSP_BOARD_LED_RED BSP_BOARD_LED_0

#define BSP_BOARD_LED_STATUS_ERROR BSP_BOARD_LED_RED
#define BSP_BOARD_LED_STATUS_OK BSP_BOARD_LED_GREEN

#define SLEEP_PERIODE_WARNING_MS 30000
#define SLEEP_PERIODE_MIN 60

#define SLEEP_PERIODE_MS SLEEP_PERIODE_MIN * 1000 * 60


void blinkTimes(int8_t times, int8_t ledBspIdx) {
	for (int var = 0; var < times; ++var) {
		bsp_board_led_on(ledBspIdx);
		nrf_delay_ms(LEDS_BLINK_PERIOD_MS);
		bsp_board_led_off(ledBspIdx);
		nrf_delay_ms(LEDS_NEXT_BLINK_PERIOD_MS);
	}
}

void sendPayloadIfNeded(int8_t value, int8_t deviceId) {
	if (deviceId > 0) {
		esbTx_sendPayload(deviceId, value);
	}
}

int main(void) {

	bsp_board_init(BSP_INIT_LEDS);
	nrf_gpio_cfg_output(LEDS_COMMON_ANODE);
	nrf_gpio_pin_write(LEDS_COMMON_ANODE, true);

	uint32_t err_code = nrf_drv_clock_init();
	APP_ERROR_CHECK(err_code);
	nrf_drv_clock_lfclk_request(NULL);

	esbTx_init();
	wakeButton_init();
	rtcModule_init();
	soilMoistureSensor_init();
	buzzer_init();

	int8_t deviceId = analogueButtonArray_get(3, 1, NRF_SAADC_INPUT_AIN1);
	int8_t sensitivity = analogueButtonArray_get(4, 2, NRF_SAADC_INPUT_AIN2);

	blinkTimes(deviceId, BSP_BOARD_LED_BLUE);
	blinkTimes(sensitivity, BSP_BOARD_LED_GREEN);

	nrf_delay_ms(LEDS_NEXT_BLINK_PERIOD_MS * 2);

	int skipCount = 0;
	while (true) {

		double voltage = batteryMeter_getVoltage();
		if (voltage < 2) {
			buzzer_beepTimes(1);

			for (int var = 0; var < 10; ++var) {
				bsp_board_led_on(BSP_BOARD_LED_STATUS_ERROR);
				rtcModule_delayMsLowPower(LEDS_BLINK_PERIOD_MS);
				bsp_board_led_off(BSP_BOARD_LED_STATUS_ERROR);
				rtcModule_delayMsLowPower(LEDS_NEXT_BLINK_PERIOD_MS);
			}
			continue;
		}

		int16_t val = soilMoistureSensor_getSoilConductivity();

		double threshood = ((100.0 / 15.0) + (double)sensitivity / 15 * (100 - (100 / 15.0 * 2)));

		bool bIsOk = val > threshood;

		bsp_board_led_on(bIsOk ? BSP_BOARD_LED_STATUS_OK : BSP_BOARD_LED_STATUS_ERROR);
		rtcModule_delayMsLowPower(LEDS_BLINK_PERIOD_MS);
		bsp_board_led_off(bIsOk ? BSP_BOARD_LED_STATUS_OK : BSP_BOARD_LED_STATUS_ERROR);
		rtcModule_delayMsLowPower(LEDS_NEXT_BLINK_PERIOD_MS);

		if (bIsOk) {
			sendPayloadIfNeded(val, deviceId);
			skipCount = 0;

			rtcModule_delayMsLowPower(SLEEP_PERIODE_MS);
		} else {
			if (skipCount) {
				skipCount--;
			} else {
				sendPayloadIfNeded(val, deviceId);
				skipCount = SLEEP_PERIODE_MS / SLEEP_PERIODE_WARNING_MS;
			}

			buzzer_beepTimes(3);

			for (int var = 0; var < 10; ++var) {
				bsp_board_led_on(BSP_BOARD_LED_STATUS_ERROR);
				rtcModule_delayMsLowPower(LEDS_BLINK_PERIOD_MS);
				bsp_board_led_off(BSP_BOARD_LED_STATUS_ERROR);
				rtcModule_delayMsLowPower(SLEEP_PERIODE_WARNING_MS / 10);
			}
		}
	}
}

