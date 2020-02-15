//
// Created by wieker on 1/29/20.
//

#ifndef BLINKY_I2CFUNC_H
#define BLINKY_I2CFUNC_H

#include <stdint-gcc.h>
#include <legacy/nrf_drv_uart.h>

extern nrf_drv_uart_t m_uart;

void twi_init (void);
void uart_init();

void scan_i2c();

uint8_t readRegister(uint8_t *reg_addr);

void initTemp();

#endif //BLINKY_I2CFUNC_H
