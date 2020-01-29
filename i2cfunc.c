#include "i2cfunc.h"
#include <legacy/nrf_drv_twi.h>
#include <boards.h>
#include <legacy/nrf_drv_uart.h>
#include <string.h>

nrf_drv_uart_t m_uart = NRF_DRV_UART_INSTANCE(0);

/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(0);

void twi_init (void)
{
    const nrf_drv_twi_config_t twi_config = {
            .scl                = ARDUINO_SCL_PIN,
            .sda                = ARDUINO_SDA_PIN,
            .frequency          = NRF_DRV_TWI_FREQ_100K,
            .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
            .clear_bus_init     = false
    };

    nrf_drv_twi_init(&m_twi, &twi_config, NULL, NULL);

    nrf_drv_twi_enable(&m_twi);
}

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

uint8_t readRegister(uint8_t *reg_addr) {
    uint8_t value;
    value = 0x00;
    nrf_drv_twi_tx(&m_twi, 0x5F, (uint8_t *) reg_addr, 1, true);
    nrf_drv_twi_rx(&m_twi, 0x5F, (uint8_t *) & value, 1);
    return value;
}

void scan_i2c() {
    ret_code_t err_code;
    uint8_t address;
    uint8_t sample_data;


    for (address = 1; address <= 127; address++)
    {
        err_code = nrf_drv_twi_rx(&m_twi, address, &sample_data, sizeof(sample_data));
        if (err_code == NRF_SUCCESS)
        {
            static char buf[100];
            sprintf(buf, "TWI device detected at address 0x%x.\r\n", address);
            nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));
        }
    }
}

void initTemp() {
    uint8_t cmd_ctrl[] = {0x20, 0x81};
    nrf_drv_twi_tx(&m_twi, 0x5F, cmd_ctrl, 2, false);
}
