
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

#define BSP_BOARD_LED_STATUS_ERROR BSP_BOARD_LED_4
#define BSP_BOARD_LED_STATUS_OK BSP_BOARD_LED_0

#define SLEEP_PERIODE_WARNING_MS 30000
#define SLEEP_PERIODE_MIN 60

#define SLEEP_PERIODE_MS SLEEP_PERIODE_MIN * 1000 * 60

int main(void) {

	bsp_board_init(BSP_INIT_LEDS);

	uint32_t err_code = nrf_drv_clock_init();
	APP_ERROR_CHECK(err_code);
	nrf_drv_clock_lfclk_request(NULL);

	esbTx_init();
	wakeButton_init();
	rtcModule_init();
	soilMoistureSensor_init();
	buzzer_init();

	int skipCount = 0;
	while (true) {

		int16_t val = soilMoistureSensor_getSoilConductivity();

		bool bIsOk = val > 3000;

		bsp_board_led_on(bIsOk ? BSP_BOARD_LED_STATUS_OK : BSP_BOARD_LED_STATUS_ERROR);
		rtcModule_delayMsLowPower(50);
		bsp_board_led_off(bIsOk ? BSP_BOARD_LED_STATUS_OK : BSP_BOARD_LED_STATUS_ERROR);

		if (bIsOk) {
			esbTx_sendPayload(val);
			skipCount = 0;

			rtcModule_delayMsLowPower(SLEEP_PERIODE_MS);
		} else {
			if (skipCount) {
				skipCount--;
			} else {
				esbTx_sendPayload(val);
				skipCount = SLEEP_PERIODE_MS / SLEEP_PERIODE_WARNING_MS;
			}

			buzzer_beepTimes(3);

			rtcModule_delayMsLowPower(SLEEP_PERIODE_WARNING_MS);
		}
	}
}

