//
// Created by wieker on 1/17/20.
//


#include <legacy/nrf_drv_uart.h>
#include <sdk_config.h>
#include <nrf_delay.h>
#include <proprietary_rf/esb/nrf_esb.h>
#include <sdk_macros.h>

nrf_drv_uart_t m_uart = NRF_DRV_UART_INSTANCE(0);

void uart_init()
{
    nrf_drv_uart_config_t config = NRF_DRV_UART_DEFAULT_CONFIG;
    config.pseltxd  = 6;
    config.pselrxd  = NRF_UART_PSEL_DISCONNECTED;
    config.pselcts  = NRF_UART_PSEL_DISCONNECTED;
    config.pselrts  = NRF_UART_PSEL_DISCONNECTED;
    config.baudrate = (nrf_uart_baudrate_t)NRF_LOG_BACKEND_UART_BAUDRATE;
    nrfx_uart_init(&m_uart.uart,
                            (nrfx_uart_config_t const *)&config,
                            NULL);
}


void clocks_start( void )
{
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}

static nrf_esb_payload_t        rx_payload;

void nrf_esb_event_handler(nrf_esb_evt_t const * p_event)
{
    switch (p_event->evt_id)
    {
        case NRF_ESB_EVENT_TX_SUCCESS:
            nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "ok \r\n", 5);
            break;
        case NRF_ESB_EVENT_TX_FAILED:
            nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "ok \r\n", 5);
            break;
        case NRF_ESB_EVENT_RX_RECEIVED:
            if (nrf_esb_read_rx_payload(&rx_payload) == NRF_SUCCESS)
            {
                // Set LEDs identical to the ones on the PTX.
                nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "rx \r\n", 5);
            }
            break;
    }
}


uint32_t esb_init( void )
{
    uint32_t err_code;
    uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
    uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };
    nrf_esb_config_t nrf_esb_config         = NRF_ESB_DEFAULT_CONFIG;
    nrf_esb_config.payload_length           = 8;
    nrf_esb_config.protocol                 = NRF_ESB_PROTOCOL_ESB_DPL;
    nrf_esb_config.bitrate                  = NRF_ESB_BITRATE_2MBPS;
    nrf_esb_config.mode                     = NRF_ESB_MODE_PRX;
    nrf_esb_config.event_handler            = nrf_esb_event_handler;
    nrf_esb_config.selective_auto_ack       = false;

    err_code = nrf_esb_init(&nrf_esb_config);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_0(base_addr_0);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_1(base_addr_1);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_prefixes(addr_prefix, 8);
    VERIFY_SUCCESS(err_code);

    return err_code;
}

int main() {
    NRF_P0->DIRSET = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 20;
    uart_init();

    clocks_start();

    esb_init();

    nrf_esb_start_rx();

    while (1) {
        NRF_P0->OUTSET = 1 << 17 | 1 << 20;
        NRF_P0->OUTCLR = 1 << 18 | 1 << 19;
        nrf_delay_ms(300);

        NRF_P0->OUTSET = 1 << 18 | 1 << 19;
        NRF_P0->OUTCLR = 1 << 17 | 1 << 20;
        nrf_delay_ms(300);

        nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "ok \r\n", 5);
    }
}

