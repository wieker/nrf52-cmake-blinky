/*
 * buzzer.h
 *
 *  Created on: May 5, 2020
 *      Author: seregia
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include <stdint.h>

void buzzer_init();
void buzzer_on();
void buzzer_off();
void buzzer_beepTimes(uint8_t times);

#endif /* BUZZER_H_ */
