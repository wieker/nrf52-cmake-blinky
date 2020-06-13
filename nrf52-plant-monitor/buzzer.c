/*
 * buzzer.c
 *
 *  Created on: May 5, 2020
 *      Author: seregia
 */

#include "buzzer.h"

#include "app_pwm.h"
#include "rtcModule.h"
#include "boards.h"

APP_PWM_INSTANCE(PWM1, 0);

static volatile bool ready_flag;

void pwm_ready_callback(uint32_t pwm_id) {
	ready_flag = true;
}

void buzzer_init(void) {

	app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH(310L, BUZZER_PIN);
	pwm1_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
	uint32_t err_code = app_pwm_init(&PWM1, &pwm1_cfg, pwm_ready_callback);
	APP_ERROR_CHECK(err_code);
}

void buzzer_on() {
	app_pwm_enable(&PWM1);
	while (app_pwm_channel_duty_set(&PWM1, 0, 50) == NRF_ERROR_BUSY);
}

void buzzer_off() {
	app_pwm_disable(&PWM1);
}

void buzzer_beepTimes(uint8_t times) {
	for (int n = 0; n < times; ++n) {
		if (n != 0) {
			rtcModule_delayMsLowPower(1000);
		}
		buzzer_on();
		rtcModule_delayMsLowPower(1000);
		buzzer_off();
	}
}
