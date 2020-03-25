//
// Created by wieker on 1/17/20.
//


#include <legacy/nrf_drv_uart.h>
#include <sdk_config.h>
#include <nrf_delay.h>
#include "boards.h"

nrf_drv_uart_t m_uart = NRF_DRV_UART_INSTANCE(0);


void uart_init()
{
    nrf_drv_uart_config_t config = NRF_DRV_UART_DEFAULT_CONFIG;
    config.pseltxd  = 30;
    config.pselrxd  = NRF_UART_PSEL_DISCONNECTED;
    config.pselcts  = NRF_UART_PSEL_DISCONNECTED;
    config.pselrts  = NRF_UART_PSEL_DISCONNECTED;
    config.baudrate = (nrf_uart_baudrate_t)NRF_LOG_BACKEND_UART_BAUDRATE;
    nrfx_uart_init(&m_uart.uart,
                            (nrfx_uart_config_t const *)&config,
                            NULL);
}

int main() {
    uart_init();
    bsp_board_init(BSP_INIT_LEDS);

    while (1) {
        bsp_board_led_on(BSP_BOARD_LED_0);
        nrf_delay_ms(200);

        bsp_board_led_off(BSP_BOARD_LED_0);
        nrf_delay_ms(500);

        nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "ok \r\n", 5);
    }
}

