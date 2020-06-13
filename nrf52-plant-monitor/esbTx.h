/*
 * esbTx.hBSP_LED_3
 *
 *  Created on: May 4, 2020
 *      Author: seregia
 */

#ifndef ESBTX_H_
#define ESBTX_H_

void esbTx_init();
void esbTx_sendPayload(int8_t deviceId, int8_t value);

#endif /* ESBTX_H_ */
