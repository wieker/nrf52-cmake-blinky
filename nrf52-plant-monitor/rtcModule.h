/*
 * rtcModule.h
 *
 *  Created on: May 4, 2020
 *      Author: seregia
 */

#ifndef RTCMODULE_H_
#define RTCMODULE_H_

void rtcModule_init();
void rtcModule_delayMsLowPower(uint32_t delayTimeMs);
void rtcModule_interruptDelay();

#endif /* RTCMODULE_H_ */
