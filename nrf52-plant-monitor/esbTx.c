/*
 * esbTx.c
 *
 *  Created on: May 4, 2020
 *      Author: seregia
 */

#include <stdbool.h>
#include "nrf_esb.h"
#include "nrf_esb_error_codes.h"
#include <nrf_delay.h>
#include "app_error.h"
#include "boards.h"

#include "esbTx.h"
#include "rtcModule.h"

#define BSP_BOARD_LED_TX_ERROR BSP_BOARD_LED_0
#define BSP_BOARD_LED_TX_ACTIVE BSP_BOARD_LED_1
#define BSP_BOARD_LED_TX_SUCCESS BSP_BOARD_LED_2

static volatile bool esb_completed = false;
static volatile bool esb_status = false;

static nrf_esb_payload_t tx_payload = NRF_ESB_CREATE_PAYLOAD(0, 0x00, 0x00);

void nrf_esb_event_handler(nrf_esb_evt_t const * p_event) {
	switch (p_event->evt_id) {
	case NRF_ESB_EVENT_TX_SUCCESS:
		esb_status = true;
		break;
	case NRF_ESB_EVENT_TX_FAILED:
		(void) nrf_esb_flush_tx();
		esb_status = false;
		break;
	default:
		esb_status = false;
		break;
	}

	esb_completed = true;
}

void esbTx_init() {
	uint32_t err_code;
	uint8_t base_addr_0[4] = { 0xE7, 0xE7, 0xE7, 0xE7 };
	uint8_t base_addr_1[4] = { 0xC2, 0xC2, 0xC2, 0xC2 };
	uint8_t addr_prefix[8] = { 0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };

	nrf_esb_config_t nrf_esb_config = NRF_ESB_DEFAULT_CONFIG;
	nrf_esb_config.tx_output_power = NRF_ESB_TX_POWER_4DBM;
	nrf_esb_config.retransmit_count = 5;
	nrf_esb_config.retransmit_delay = 0xFFFF;

	nrf_esb_config.selective_auto_ack = false;
	nrf_esb_config.protocol = NRF_ESB_PROTOCOL_ESB_DPL;
	nrf_esb_config.bitrate = NRF_ESB_BITRATE_1MBPS;
	nrf_esb_config.event_handler = nrf_esb_event_handler;
	nrf_esb_config.mode = NRF_ESB_MODE_PTX;

	err_code = nrf_esb_init(&nrf_esb_config);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_esb_set_base_address_0(base_addr_0);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_esb_set_base_address_1(base_addr_1);
	APP_ERROR_CHECK(err_code);

	err_code = nrf_esb_set_prefixes(addr_prefix, 8);
	APP_ERROR_CHECK(err_code);

	tx_payload.length = 2;
	tx_payload.pipe = 0;
	tx_payload.data[0] = 0x00;
	tx_payload.data[1] = 0x00;

	esb_completed = true;
}

void esbTx_sendPayload(int8_t deviceId, int8_t value) {
	bsp_board_led_on(BSP_BOARD_LED_TX_ACTIVE);
	rtcModule_delayMsLowPower(LEDS_BLINK_PERIOD_MS);

	tx_payload.data[0] = deviceId;
	tx_payload.data[1] = value;
	tx_payload.noack = false;
	esb_completed = false;

	uint32_t err_code = nrf_esb_write_payload(&tx_payload);
	APP_ERROR_CHECK(err_code);

	while (!esb_completed);

	bsp_board_led_off(BSP_BOARD_LED_TX_ACTIVE);
	rtcModule_delayMsLowPower(LEDS_NEXT_BLINK_PERIOD_MS);

	bsp_board_led_on(esb_status ? BSP_BOARD_LED_TX_SUCCESS : BSP_BOARD_LED_TX_ERROR);
	rtcModule_delayMsLowPower(LEDS_BLINK_PERIOD_MS);
	bsp_board_led_off(esb_status ? BSP_BOARD_LED_TX_SUCCESS : BSP_BOARD_LED_TX_ERROR);
}
