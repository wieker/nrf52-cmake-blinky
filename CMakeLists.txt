cmake_minimum_required(VERSION 3.12)
project(blinky C ASM)

set(CMAKE_VERBOSE_MAKEFILE 1)

set(NRF52_SDK "/home/wieker/Projects/linux/nrf/sdk")
# or
# -DNRF52_SDK=...

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")
set(CMAKE_ASM_COMPILER "/usr/bin/arm-none-eabi-gcc")

set(ARCH_FLAGS "\
        -mabi=aapcs \
        -mcpu=cortex-m4 \
        -mfloat-abi=hard \
        -mfpu=fpv4-sp-d16 \
        -mthumb \
        -g3 \
")

set(GLOBAL_FLAGS "${ARCH_FLAGS} \
        -DNRF52 \
        -DNRF52832_XXAA \
        -DBOARD_PCA10040 \
")

set(CMAKE_ASM_FLAGS "${GLOBAL_FLAGS} \
        -x assembler-with-cpp \
")

set(CMAKE_C_FLAGS "${GLOBAL_FLAGS} \
        -O3 \
        -Wall \
        -Werror \
        -fdata-sections \
        -ffunction-sections \
        -fno-builtin \
        -fno-strict-aliasing \
        -fshort-enums \
        -std=c99 \
")

include_directories(
        ${NRF52_SDK}/components/toolchain/cmsis/include
        ${NRF52_SDK}/modules/nrfx/mdk

        ${NRF52_SDK}/components/libraries/delay
        ${NRF52_SDK}/modules/nrfx
        ${NRF52_SDK}/modules/nrfx/templates/nRF52832  # nrfx_config.h
        ${NRF52_SDK}/integration/nrfx
        ${NRF52_SDK}/components/libraries/util

        ble_config

        ${NRF52_SDK}/components/nfc/ndef/generic/message
        ${NRF52_SDK}/components/nfc/t2t_lib
        ${NRF52_SDK}/components/nfc/t4t_parser/hl_detection_procedure
        ${NRF52_SDK}/components/ble/ble_services/ble_ancs_c
        ${NRF52_SDK}/components/ble/ble_services/ble_ias_c
        ${NRF52_SDK}/components/libraries/pwm
        ${NRF52_SDK}/components/softdevice/s132/headers/nrf52
        ${NRF52_SDK}/components/libraries/usbd/class/cdc/acm
        ${NRF52_SDK}/components/libraries/usbd/class/hid/generic
        ${NRF52_SDK}/components/libraries/usbd/class/msc
        ${NRF52_SDK}/components/libraries/usbd/class/hid
        ${NRF52_SDK}/modules/nrfx/hal
        ${NRF52_SDK}/components/libraries/log
        ${NRF52_SDK}/components/ble/ble_services/ble_gls
        ${NRF52_SDK}/components/libraries/fstorage
        ${NRF52_SDK}/components/nfc/ndef/text
        ${NRF52_SDK}/components/libraries/mutex
        ${NRF52_SDK}/components/libraries/gpiote
        ${NRF52_SDK}/components/libraries/log/src
        ${NRF52_SDK}/components/libraries/bootloader/ble_dfu
        ${NRF52_SDK}/components/nfc/ndef/connection_handover/common
        ${NRF52_SDK}/components/boards
        ${NRF52_SDK}/components/nfc/ndef/generic/record
        ${NRF52_SDK}/components/libraries/memobj
        ${NRF52_SDK}/components/nfc/t4t_parser/cc_file
        ${NRF52_SDK}/components/ble/ble_advertising
        ${NRF52_SDK}/components/ble/ble_services/ble_bas_c
        ${NRF52_SDK}/modules/nrfx/drivers/include
        ${NRF52_SDK}/components/libraries/experimental_task_manager
        ${NRF52_SDK}/components/ble/ble_services/ble_hrs_c
        ${NRF52_SDK}/components/nfc/ndef/connection_handover/le_oob_rec
        ${NRF52_SDK}/components/libraries/queue
        ${NRF52_SDK}/components/libraries/pwr_mgmt
        ${NRF52_SDK}/components/ble/ble_dtm
        ${NRF52_SDK}/components/toolchain/cmsis/include
        ${NRF52_SDK}/components/ble/ble_services/ble_rscs_c
        ${NRF52_SDK}/components/ble/common
        ${NRF52_SDK}/components/ble/ble_services/ble_lls
        ${NRF52_SDK}/components/libraries/bsp
        ${NRF52_SDK}/components/nfc/ndef/connection_handover/ac_rec
        ${NRF52_SDK}/components/ble/ble_services/ble_bas
        ${NRF52_SDK}/components/libraries/experimental_section_vars
        ${NRF52_SDK}/components/softdevice/s132/headers
        ${NRF52_SDK}/components/ble/ble_services/ble_ans_c
        ${NRF52_SDK}/components/libraries/slip
        ${NRF52_SDK}/components/libraries/delay
        ${NRF52_SDK}/components/libraries/experimental_mpu
        ${NRF52_SDK}/components/libraries/mem_manager
        ${NRF52_SDK}/components/libraries/csense_drv
        ${NRF52_SDK}/components/ble/ble_services/ble_nus_c
        ${NRF52_SDK}/components/libraries/usbd/config
        ${NRF52_SDK}/components/softdevice/common
        ${NRF52_SDK}/components/ble/ble_services/ble_ias
        ${NRF52_SDK}/components/libraries/usbd/class/hid/mouse
        ${NRF52_SDK}/components/libraries/low_power_pwm
        ${NRF52_SDK}/components/nfc/ndef/conn_hand_parser/ble_oob_advdata_parser
        ${NRF52_SDK}/components/ble/ble_services/ble_dfu
        ${NRF52_SDK}/external/fprintf
        ${NRF52_SDK}/components/libraries/atomic
        ${NRF52_SDK}/components
        ${NRF52_SDK}/components/libraries/scheduler
        ${NRF52_SDK}/components/libraries/cli
        ${NRF52_SDK}/components/ble/ble_services/ble_lbs
        ${NRF52_SDK}/components/ble/ble_services/ble_hts
        ${NRF52_SDK}/components/libraries/crc16
        ${NRF52_SDK}/components/nfc/t4t_parser/apdu
        ${NRF52_SDK}/components/libraries/util
        ${NRF52_SDK}/components/libraries/usbd/class/cdc
        ${NRF52_SDK}/components/libraries/csense
        ${NRF52_SDK}/components/libraries/balloc
        ${NRF52_SDK}/components/libraries/ecc
        ${NRF52_SDK}/components/libraries/hardfault
        ${NRF52_SDK}/components/ble/ble_services/ble_cscs
        ${NRF52_SDK}/components/libraries/hci
        ${NRF52_SDK}/components/libraries/timer
        ${NRF52_SDK}/integration/nrfx
        ${NRF52_SDK}/components/nfc/t4t_parser/tlv
        ${NRF52_SDK}/components/libraries/sortlist
        ${NRF52_SDK}/components/libraries/spi_mngr
        ${NRF52_SDK}/components/libraries/experimental_stack_guard
        ${NRF52_SDK}/components/libraries/led_softblink
        ${NRF52_SDK}/components/nfc/ndef/conn_hand_parser
        ${NRF52_SDK}/components/libraries/sdcard
        ${NRF52_SDK}/components/nfc/ndef/parser/record
        ${NRF52_SDK}/modules/nrfx/mdk
        ${NRF52_SDK}/components/ble/ble_services/ble_cts_c
        ${NRF52_SDK}/components/nfc/ndef/conn_hand_parser/le_oob_rec_parser
        ${NRF52_SDK}/components/ble/ble_services/ble_nus
        ${NRF52_SDK}/components/libraries/twi_mngr
        ${NRF52_SDK}/components/ble/ble_services/ble_hids
        ${NRF52_SDK}/components/libraries/strerror
        ${NRF52_SDK}/components/libraries/crc32
        ${NRF52_SDK}/components/nfc/ndef/connection_handover/ble_oob_advdata
        ${NRF52_SDK}/components/nfc/t2t_parser
        ${NRF52_SDK}/components/nfc/ndef/connection_handover/ble_pair_msg
        ${NRF52_SDK}/components/libraries/usbd/class/audio
        ${NRF52_SDK}/components/nfc/t4t_lib/hal_t4t
        ${NRF52_SDK}/components/libraries/sensorsim
        ${NRF52_SDK}/components/nfc/t4t_lib
        ${NRF52_SDK}/components/ble/peer_manager
        ${NRF52_SDK}/components/drivers_nrf/usbd
        ${NRF52_SDK}/components/ble/ble_services/ble_tps
        ${NRF52_SDK}/components/nfc/ndef/parser/message
        ${NRF52_SDK}/components/ble/ble_services/ble_dis
        ${NRF52_SDK}/components/nfc/ndef/uri
        ${NRF52_SDK}/components/ble/nrf_ble_gatt
        ${NRF52_SDK}/components/ble/nrf_ble_qwr
        ${NRF52_SDK}/components/libraries/gfx
        ${NRF52_SDK}/components/libraries/button
        ${NRF52_SDK}/modules/nrfx
        ${NRF52_SDK}/components/libraries/twi_sensor
        ${NRF52_SDK}/integration/nrfx/legacy
        ${NRF52_SDK}/components/libraries/usbd/class/hid/kbd
        ${NRF52_SDK}/components/nfc/ndef/connection_handover/ep_oob_rec
        ${NRF52_SDK}/external/segger_rtt
        ${NRF52_SDK}/components/libraries/atomic_fifo
        ${NRF52_SDK}/components/libraries/experimental_ringbuf
        ${NRF52_SDK}/components/ble/ble_services/ble_lbs_c
        ${NRF52_SDK}/components/nfc/ndef/connection_handover/ble_pair_lib
        ${NRF52_SDK}/components/libraries/crypto
        ${NRF52_SDK}/components/ble/ble_racp
        ${NRF52_SDK}/components/libraries/fds
        ${NRF52_SDK}/components/nfc/ndef/launchapp
        ${NRF52_SDK}/components/libraries/atomic_flags
        ${NRF52_SDK}/components/ble/ble_services/ble_rscs
        ${NRF52_SDK}/components/nfc/ndef/connection_handover/hs_rec
        ${NRF52_SDK}/components/nfc/t2t_lib/hal_t2t
        ${NRF52_SDK}/components/libraries/usbd
        ${NRF52_SDK}/components/nfc/ndef/conn_hand_parser/ac_rec_parser
        ${NRF52_SDK}/components/ble/ble_services/ble_hrs

        ${NRF52_SDK}/components/libraries/ringbuf
)

set(CMAKE_EXE_LINKER_FLAGS "${ARCH_FLAGS} \
        --specs=nano.specs \
        -T${PROJECT_SOURCE_DIR}/blinky.ld \
        -Wl,--gc-sections \
        -L${NRF52_SDK}/modules/nrfx/mdk \
        -lc \
        -lnosys \
        -lm \
")

set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <LINK_FLAGS> <OBJECTS> -o <TARGET>")

add_executable(${CMAKE_PROJECT_NAME}
        main.c
        ${NRF52_SDK}/modules/nrfx/mdk/gcc_startup_nrf52.S
        ${NRF52_SDK}/modules/nrfx/mdk/system_nrf52.c
)

add_executable(ble
        ble.c
        ${NRF52_SDK}/modules/nrfx/mdk/gcc_startup_nrf52.S
        ${NRF52_SDK}/modules/nrfx/mdk/system_nrf52.c


        ${NRF52_SDK}/components/libraries/log/src/nrf_log_backend_rtt.c
        ${NRF52_SDK}/components/libraries/log/src/nrf_log_backend_serial.c
        ${NRF52_SDK}/components/libraries/log/src/nrf_log_backend_uart.c
        ${NRF52_SDK}/components/libraries/log/src/nrf_log_default_backends.c
        ${NRF52_SDK}/components/libraries/log/src/nrf_log_frontend.c
        ${NRF52_SDK}/components/libraries/log/src/nrf_log_str_formatter.c
        ${NRF52_SDK}/components/libraries/button/app_button.c
        ${NRF52_SDK}/components/libraries/util/app_error.c
        ${NRF52_SDK}/components/libraries/util/app_error_handler_gcc.c
        ${NRF52_SDK}/components/libraries/util/app_error_weak.c
        ${NRF52_SDK}/components/libraries/scheduler/app_scheduler.c
        ${NRF52_SDK}/components/libraries/timer/app_timer.c
        ${NRF52_SDK}/components/libraries/util/app_util_platform.c
        ${NRF52_SDK}/components/libraries/crc16/crc16.c
        ${NRF52_SDK}/components/libraries/hardfault/hardfault_implementation.c
        ${NRF52_SDK}/components/libraries/util/nrf_assert.c
        ${NRF52_SDK}/components/libraries/atomic_fifo/nrf_atfifo.c
        ${NRF52_SDK}/components/libraries/atomic_flags/nrf_atflags.c
        ${NRF52_SDK}/components/libraries/atomic/nrf_atomic.c
        ${NRF52_SDK}/components/libraries/balloc/nrf_balloc.c
        ${NRF52_SDK}/external/fprintf/nrf_fprintf.c
        ${NRF52_SDK}/external/fprintf/nrf_fprintf_format.c
        ${NRF52_SDK}/components/libraries/memobj/nrf_memobj.c
        ${NRF52_SDK}/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c
        ${NRF52_SDK}/components/libraries/experimental_section_vars/nrf_section_iter.c
        ${NRF52_SDK}/components/libraries/strerror/nrf_strerror.c
        ${NRF52_SDK}/components/libraries/sensorsim/sensorsim.c
        ${NRF52_SDK}/modules/nrfx/mdk/system_nrf52.c
        ${NRF52_SDK}/components/boards/boards.c
        ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_clock.c
        ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_uart.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_clock.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_gpiote.c
        #${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_power_clock.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/prs/nrfx_prs.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_uart.c
        ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_uarte.c
        ${NRF52_SDK}/components/libraries/bsp/bsp.c
        ${NRF52_SDK}/components/libraries/bsp/bsp_btn_ble.c
        ${NRF52_SDK}/external/segger_rtt/SEGGER_RTT.c
        ${NRF52_SDK}/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c
        ${NRF52_SDK}/external/segger_rtt/SEGGER_RTT_printf.c
        ${NRF52_SDK}/components/ble/common/ble_advdata.c
        ${NRF52_SDK}/components/ble/ble_advertising/ble_advertising.c
        ${NRF52_SDK}/components/ble/common/ble_conn_params.c
        ${NRF52_SDK}/components/ble/common/ble_conn_state.c
        ${NRF52_SDK}/components/ble/common/ble_srv_common.c
        ${NRF52_SDK}/components/ble/peer_manager/gatt_cache_manager.c
        ${NRF52_SDK}/components/ble/peer_manager/gatts_cache_manager.c
        ${NRF52_SDK}/components/ble/nrf_ble_gatt/nrf_ble_gatt.c
        ${NRF52_SDK}/components/ble/nrf_ble_qwr/nrf_ble_qwr.c
        ${NRF52_SDK}/components/ble/peer_manager/peer_id.c
        ${NRF52_SDK}/components/ble/peer_manager/pm_buffer.c
        #${NRF52_SDK}/components/ble/peer_manager/pm_mutex.c
        ${NRF52_SDK}/components/ble/peer_manager/security_dispatcher.c
        ${NRF52_SDK}/components/softdevice/common/nrf_sdh.c
        ${NRF52_SDK}/components/softdevice/common/nrf_sdh_ble.c
        ${NRF52_SDK}/components/softdevice/common/nrf_sdh_soc.c

        ${NRF52_SDK}/components/ble/ble_services/ble_lbs/ble_lbs.c
        ${NRF52_SDK}/components/libraries/ringbuf/nrf_ringbuf.c
        )

add_custom_command(
        OUTPUT ${CMAKE_PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O ihex ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}.hex
        DEPENDS ${CMAKE_PROJECT_NAME}
)

add_custom_target(
        hex
        DEPENDS ${CMAKE_PROJECT_NAME}.hex
)

add_custom_target(
        flash
        DEPENDS ${CMAKE_PROJECT_NAME}.hex
        COMMAND nrfjprog -f nrf52 --program ${CMAKE_PROJECT_NAME}.hex --sectorerase --reset
)

add_custom_target(
        flash_softdevice
        COMMAND nrfjprog -f nrf52 --program ${NRF52_SDK}/components/softdevice/s132/hex/s132_nrf52_6.1.0_softdevice.hex --sectorerase
)

add_custom_target(
        reset
        COMMAND nrfjprog -f nrf52 --reset
)

add_custom_target(
        erase
        COMMAND nrfjprog -f nrf52 --eraseall
)
