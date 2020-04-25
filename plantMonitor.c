/**
 * Copyright (c) 2014 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "nrf_esb.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "sdk_common.h"
#include "nrf.h"
#include "nrf_error.h"
#include "nrf_esb_error_codes.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "app_util.h"
#include <nrf_delay.h>

#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"
#include "nrf_soc.h"

//#include "app_pwm.h"

#include "soilMoistureSensor.h"

//APP_PWM_INSTANCE(PWM1,0);                   // Create the instance "PWM1" using TIMER1.


static volatile bool ready_flag;            // A flag indicating PWM status.

void pwm_ready_callback(uint32_t pwm_id)    // PWM callback function
{
    ready_flag = true;
}

#define COMPARE_COUNTERTIME  (3UL)                                        /**< Get Compare event COMPARE_TIME seconds after the counter starts from 0. */

#ifdef BSP_LED_2
    #define TICK_EVENT_OUTPUT     BSP_LED_2                                 /**< Pin number for indicating tick event. */
#endif
#ifndef TICK_EVENT_OUTPUT
    #error "Please indicate output pin"
#endif
#ifdef BSP_LED_3
    #define COMPARE_EVENT_OUTPUT   BSP_LED_3                                /**< Pin number for indicating compare event. */
#endif
#ifndef COMPARE_EVENT_OUTPUT
    #error "Please indicate output pin"
#endif


#define RESET_MEMORY_TEST_BYTE  (0x0DUL)        /**< Known sequence written to a special register to check if this wake up is from System OFF. */
#define RAM_RETENTION_OFF       (0x00000003UL)  /**< The flag used to turn off RAM retention on nRF52. */

#define BTN_PRESSED     0                       /**< Value of a pressed button. */
#define BTN_RELEASED    1                       /**< Value of a released button. */

//#define NRF_ESB_LEGACY

/*lint -save -esym(40, BUTTON_1) -esym(40, BUTTON_2) -esym(40, BUTTON_3) -esym(40, BUTTON_4) -esym(40, LED_1) -esym(40, LED_2) -esym(40, LED_3) -esym(40, LED_4) */

static nrf_esb_payload_t tx_payload = NRF_ESB_CREATE_PAYLOAD(0, 0x01, 0x00);
static nrf_esb_payload_t rx_payload;
static uint32_t button_state_1;
static volatile bool esb_completed = false;

void system_off( void )
{
#ifdef NRF51
    NRF_POWER->RAMON |= (POWER_RAMON_OFFRAM0_RAM0Off << POWER_RAMON_OFFRAM0_Pos) |
                        (POWER_RAMON_OFFRAM1_RAM1Off << POWER_RAMON_OFFRAM1_Pos);
#endif //NRF51
#ifdef NRF52
    NRF_POWER->RAM[0].POWER = RAM_RETENTION_OFF;
    NRF_POWER->RAM[1].POWER = RAM_RETENTION_OFF;
    NRF_POWER->RAM[2].POWER = RAM_RETENTION_OFF;
    NRF_POWER->RAM[3].POWER = RAM_RETENTION_OFF;
    NRF_POWER->RAM[4].POWER = RAM_RETENTION_OFF;
    NRF_POWER->RAM[5].POWER = RAM_RETENTION_OFF;
    NRF_POWER->RAM[6].POWER = RAM_RETENTION_OFF;
    NRF_POWER->RAM[7].POWER = RAM_RETENTION_OFF;
#endif //NRF52

    // Turn off LEDs before sleeping to conserve energy.
    bsp_board_leds_off();

    // Set nRF5 into System OFF. Reading out value and looping after setting the register
    // to guarantee System OFF in nRF52.
    NRF_POWER->SYSTEMOFF = 0x1;
    (void) NRF_POWER->SYSTEMOFF;
    while (true);
}


void nrf_esb_event_handler(nrf_esb_evt_t const * p_event)
{
    switch (p_event->evt_id)
    {
        case NRF_ESB_EVENT_TX_SUCCESS:
        	bsp_board_led_on(BSP_BOARD_LED_1);
        	nrf_delay_ms(50);
        	bsp_board_led_off(BSP_BOARD_LED_1);
            break;
        case NRF_ESB_EVENT_TX_FAILED:
        	bsp_board_led_on(BSP_BOARD_LED_4);
        	nrf_delay_ms(50);
        	bsp_board_led_off(BSP_BOARD_LED_4);
            (void) nrf_esb_flush_tx();
            break;
        case NRF_ESB_EVENT_RX_RECEIVED:
            // Get the most recent element from the RX FIFO.
            while (nrf_esb_read_rx_payload(&rx_payload) == NRF_SUCCESS) ;

            // For each LED, set it as indicated in the rx_payload, but invert it for the button
            // which is pressed. This is because the ack payload from the PRX is reflecting the
            // state from before receiving the packet.
            nrf_gpio_pin_write(LED_1, !( ((rx_payload.data[0] & 0x01) == 0) ^ (button_state_1 == BTN_PRESSED)) );
            break;
    }

    esb_completed = true;
}


void clocks_start( void )
{
    // Start HFCLK and wait for it to start.
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);
}


uint32_t esb_init( void )
{
    uint32_t err_code;
    uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
    uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };

#ifndef NRF_ESB_LEGACY
    nrf_esb_config_t nrf_esb_config         = NRF_ESB_DEFAULT_CONFIG;
#else // NRF_ESB_LEGACY
    nrf_esb_config_t nrf_esb_config         = NRF_ESB_LEGACY_CONFIG;
#endif // NRF_ESB_LEGACY
    nrf_esb_config.tx_output_power          = NRF_ESB_TX_POWER_4DBM;
    nrf_esb_config.retransmit_count         = 15;
    nrf_esb_config.selective_auto_ack       = false;
    nrf_esb_config.protocol                 = NRF_ESB_PROTOCOL_ESB_DPL;
    nrf_esb_config.bitrate                  = NRF_ESB_BITRATE_1MBPS;
    nrf_esb_config.event_handler            = nrf_esb_event_handler;
    nrf_esb_config.mode                     = NRF_ESB_MODE_PTX;

    err_code = nrf_esb_init(&nrf_esb_config);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_0(base_addr_0);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_1(base_addr_1);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_prefixes(addr_prefix, 8);
    VERIFY_SUCCESS(err_code);

    tx_payload.length  = 3;
    tx_payload.pipe    = 0;
    tx_payload.data[0] = 0x00;

    return NRF_SUCCESS;
}


uint32_t gpio_check_and_esb_tx()
{
    uint32_t err_code;
    button_state_1 = nrf_gpio_pin_read(BUTTON_1);

    nrf_gpio_pin_write(LED_1, !button_state_1);


    tx_payload.data[0] = button_state_1 == BTN_PRESSED ? '1' : '0';

    //if (button_state_1 == BTN_PRESSED + 1 * BTN_RELEASED)
    {
        tx_payload.noack = false;
        err_code = nrf_esb_write_payload(&tx_payload);
        VERIFY_SUCCESS(err_code);
    }
//    else
//    {
//        esb_completed = true;
//    }

    return NRF_SUCCESS;
}


void gpio_init( void )
{
    nrf_gpio_cfg_sense_input(BUTTON_1, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
    // Workaround for PAN_028 rev1.1 anomaly 22 - System: Issues with disable System OFF mechanism
    nrf_delay_ms(1);

    bsp_board_init(BSP_INIT_LEDS);
    //soilMoistureSensor_setup();
}


void recover_state()
{
    uint32_t            loop_count = 0;
    if ((NRF_POWER->GPREGRET >> 4) == RESET_MEMORY_TEST_BYTE)
    {
        // Take the loop_count value.
        loop_count          = (uint8_t)(NRF_POWER->GPREGRET & 0xFUL);
        NRF_POWER->GPREGRET = 0;
    }

    loop_count++;
    NRF_POWER->GPREGRET = ( (RESET_MEMORY_TEST_BYTE << 4) | loop_count);

    tx_payload.data[1] = loop_count << 4;
}

// ########## RTC ###########

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(0); /**< Declaring an instance of nrf_drv_rtc for RTC0. */

/** @brief: Function for handling the RTC0 interrupts.
 * Triggered on TICK and COMPARE0 match.
 */
static void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{
    if (int_type == NRF_DRV_RTC_INT_COMPARE0)
    {
        nrf_gpio_pin_toggle(COMPARE_EVENT_OUTPUT);
        esb_completed = false;
        //NVIC_EnableIRQ(POWER_CLOCK_IRQn); // do this in main
        clocks_start();
        nrf_delay_ms(50);
        NRF_POWER->TASKS_LOWPWR = 0;
        nrf_delay_ms(50);
        clocks_start();
        //NVIC_EnableIRQ(POWER_CLOCK_IRQn); // do this in main

        esb_init();
        gpio_init();

        // Recover state if the device was woken from System OFF.
        recover_state();

//        nrf_gpio_cfg_output(4);
//
//        for (int var = 0; var < 1000; ++var) {
//
//        	nrf_gpio_pin_write(4, 1);
//        	nrf_delay_us(100);
//        	nrf_gpio_pin_write(4, 0);
//        	nrf_delay_us(100);
//		}

        //app_pwm_enable(&PWM1);
        //while (app_pwm_channel_duty_set(&PWM1, 0, 50) == NRF_ERROR_BUSY);
        //nrf_delay_ms(50);
        //app_pwm_disable(&PWM1);

        //nrf_gpio_pin_write(BSP_LED_2, soilMoistureSensor_hasWater());

        uint32_t err_code = gpio_check_and_esb_tx();
        APP_ERROR_CHECK(err_code);

        while (!esb_completed);

        // Turn off LEDs before sleeping to conserve energy.
        bsp_board_leds_off();

        nrfx_rtc_counter_clear(&rtc);
        nrf_drv_rtc_cc_set(&rtc,0,0xFFFF,true);
    }
    else if (int_type == NRF_DRV_RTC_INT_TICK)
    {
        nrf_gpio_pin_toggle(TICK_EVENT_OUTPUT);
    }
}

/** @brief Function starting the internal LFCLK XTAL oscillator.
 */
static void lfclk_config(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
}


/** @brief Function initialization and configuration of RTC driver instance.
 */
static void rtc_config(void)
{
    uint32_t err_code;

    //Initialize RTC instance
    nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
    //config.prescaler = 4095;
    config.prescaler = 0;
    err_code = nrf_drv_rtc_init(&rtc, &config, rtc_handler);
    APP_ERROR_CHECK(err_code);

    //Enable tick event & interrupt
    //nrf_drv_rtc_tick_enable(&rtc,false);

    //Set compare channel to trigger interrupt after COMPARE_COUNTERTIME seconds
    err_code = nrf_drv_rtc_cc_set(&rtc,0,0xFFFF,true);
    APP_ERROR_CHECK(err_code);

    //Power on RTC instance
    nrf_drv_rtc_enable(&rtc);
}

int main(void)
{
    uint32_t err_code;
    // Initialize
    clocks_start();
    err_code = esb_init();
    APP_ERROR_CHECK(err_code);

    gpio_init();

   // nrf_gpio_cfg_output(5);


    /* 2-channel PWM, 200Hz, output on DK LED pins. */
    //app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH(310L, 4);

    /* Switch the polarity of the second channel. */
    //pwm1_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;

    /* Initialize and enable PWM. */
    //err_code =
    //		app_pwm_init(&PWM1,&pwm1_cfg,pwm_ready_callback);
    //APP_ERROR_CHECK(err_code);
    //app_pwm_enable(&PWM1);

    //while (app_pwm_channel_duty_set(&PWM1, 0, 50) == NRF_ERROR_BUSY);
    //app_pwm_disable(&PWM1);
    //while (true);

    // Recover state if the device was woken from System OFF.
    recover_state();



    lfclk_config();
    rtc_config();
    esb_completed = true;

   // sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
     //   sd_ble_gap_tx_power_set(0);
    while (true)
    {
    	while (!esb_completed);
    	//NVIC_EnableIRQ(POWER_CLOCK_IRQn); // do this in main

    	NRF_POWER->TASKS_LOWPWR = 1;

    	//NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
		NRF_CLOCK->TASKS_HFCLKSTOP = 1;
		//while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 1);

        __SEV();
        __WFE();
        __WFE();
        //NVIC_EnableIRQ(POWER_CLOCK_IRQn); // do this in main
    }



    // Check state of all buttons and send an esb packet with the button press if there is exactly one.
    err_code = gpio_check_and_esb_tx();
    APP_ERROR_CHECK(err_code);

    while (true)
    {
        // Wait for esb completed and all buttons released before going to system off.
        if (esb_completed)
        {
            if (nrf_gpio_pin_read(BUTTON_1) == BTN_RELEASED)
            {
            	esb_completed = false;
                err_code = gpio_check_and_esb_tx();
                APP_ERROR_CHECK(err_code);
                while (!esb_completed);
                system_off();
            }
        }
    }
}
/*lint -restore */

