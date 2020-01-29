//
// Created by wieker on 1/18/20.
//


#include <legacy/nrf_drv_uart.h>
#include <sdk_config.h>
#include <nrf_delay.h>
#include <legacy/nrf_drv_twi.h>
#include <pca10040.h>
#include <string.h>
#include "i2cfunc.h"

void k_uart_write(const char *buf);

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

__STATIC_INLINE void data_handler(uint8_t temp)
{
    static char buf[100];
    sprintf(buf, "Temperature: %d Celsius degrees.\r\n", temp);
    nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));
}

int main() {
    NRF_P0->DIRSET = 1 << 17 | 1 << 18 | 1 << 19 | 1 << 20;
    uart_init();
    twi_init();
    int a = 0;


    initTemp();

    uint8_t addr16 = 0x20;
    uint8_t value = readRegister(&addr16);
    static char buf[100];
    sprintf(buf, "Response Control: 0x%x.\r\n", value);
    nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));


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


        addr16 = 0x0F;
        value = readRegister(&addr16);
        static char buf[100];
        sprintf(buf, "Response: 0x%x.\r\n", value);
        nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));
        int t0, t1, cvt0, cvt1;


        addr16 = 0x32;
        value = readRegister(&addr16);
        sprintf(buf, "T0_degC_x8: %d.\r\n", value >> 3);
        nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));
        t0 = value >> 3;


        addr16 = 0x33;
        value = readRegister(&addr16);
        sprintf(buf, "T1_degC_x8: %d.\r\n", value >> 3);
        nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));
        t1 = value >> 3;
        addr16 = 0x35;
        value = readRegister(&addr16);
        sprintf(buf, "T1_degC_x8: %d.\r\n", value >> 3);
        nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));
        t1 += (((value >> 2) & 0x3) << 5);


        addr16 = 0x3C;
        value = readRegister(&addr16);
        addr16 = 0x3D;
        int8_t valueHigh = readRegister(&addr16);

        sprintf(buf, "T0_OUT: %d.\r\n", (valueHigh << 8) + value);
        nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));
        cvt0 = (valueHigh << 8) + value;

        addr16 = 0x3E;
        value = readRegister(&addr16);
        addr16 = 0x3F;
        valueHigh = readRegister(&addr16);

        sprintf(buf, "T1_OUT: %d.\r\n", (valueHigh << 8) + value);
        nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));
        cvt1 = (valueHigh << 8) + value;

        addr16 = 0x2A;
        value = readRegister(&addr16);
        addr16 = 0x2B;
        valueHigh = readRegister(&addr16);

        sprintf(buf, "Temperature: %d.\r\n", (valueHigh << 8) + value);
        nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));
        int raw = (valueHigh << 8) + value;
        sprintf(buf, "Temperature: %d.\r\n", ((t1 - t0) * (raw - cvt0)) / (cvt1 - cvt0) + t0);
        nrfx_uart_tx(&m_uart.uart, (uint8_t  *) buf, strlen(buf));
    }
}

