//
// Created by wieker on 1/18/20.
//


#include <legacy/nrf_drv_uart.h>
#include <sdk_config.h>
#include <nrf_delay.h>
#include <legacy/nrf_drv_twi.h>
#include <pca10040.h>
#include <string.h>

nrf_drv_uart_t m_uart = NRF_DRV_UART_INSTANCE(0);

void scan_i2c();

/* TWI instance. */
static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(0);

void uart_init()
{
    nrf_drv_uart_config_t config = NRF_DRV_UART_DEFAULT_CONFIG;
    config.pseltxd  = 11;
    config.pselrxd  = NRF_UART_PSEL_DISCONNECTED;
    config.pselcts  = NRF_UART_PSEL_DISCONNECTED;
    config.pselrts  = NRF_UART_PSEL_DISCONNECTED;
    config.baudrate = (nrf_uart_baudrate_t)NRF_LOG_BACKEND_UART_BAUDRATE;
    nrfx_uart_init(&m_uart.uart,
                   (nrfx_uart_config_t const *)&config,
                   NULL);
}

__STATIC_INLINE void data_handler(uint8_t temp)
{
    static char buf[100];
    sprintf(buf, "Temperature: %d Celsius degrees.\r\n", temp);
    nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));
}

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

int main() {
    NRF_P0->DIRSET = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 20;
    uart_init();
    twi_init();
    int a = 0;


    while (1) {
        NRF_P0->OUTSET = 1 << 17 | 1 << 20;
        NRF_P0->OUTCLR = 1 << 18 | 1 << 19;
        nrf_delay_ms(300);

        NRF_P0->OUTSET = 1 << 18 | 1 << 19;
        NRF_P0->OUTCLR = 1 << 17 | 1 << 20;
        nrf_delay_ms(300);

        nrfx_uart_tx(&m_uart.uart, (uint8_t  *) "ok \r\n", 5);
        data_handler(a ++);
        scan_i2c();
    }
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

