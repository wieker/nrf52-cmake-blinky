cmake_minimum_required(VERSION 3.5.1)
project(blinky C ASM)

set(CMAKE_VERBOSE_MAKEFILE 1)

set(NRF52_SDK "/opt/nRF5_SDK_for_Thread_and_Zigbee_v4.0.0_dc7186b")
# or
# -DNRF52_SDK=...

set(OPENOCD_BINARY "openocd")
set(CMAKE_C_STANDARD 99)
set(SELECTED_TARGET light_bulb)
set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")
set(CMAKE_ASM_COMPILER "/usr/bin/arm-none-eabi-gcc")

set(ARCH_FLAGS "\
        -mabi=aapcs \
        -mcpu=cortex-m4 \
        -mthumb \
        -g3 \
        -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
")

set(GLOBAL_FLAGS "${ARCH_FLAGS} \
        -DBOARD_PCA10059 \
        -DNRF52840_XXAA \
        -DAPP_TIMER_V2 \
        -DAPP_TIMER_V2_RTC1_ENABLED \
        -DCONFIG_GPIO_AS_PINRESET \
        -DENABLE_FEM \
        -DFLOAT_ABI_HARD \
        -DZB_TRACE_LEVEL=0 \
        -DZB_TRACE_MASK=0 \
")

set(CMAKE_ASM_FLAGS "${GLOBAL_FLAGS} \
        -x assembler-with-cpp \
")

set(CMAKE_C_FLAGS "${GLOBAL_FLAGS} \
        -Wall \
        -Werror \
        -fdata-sections \
        -ffunction-sections \
        -fno-builtin \
        -fno-strict-aliasing \
        -fshort-enums \
        -std=c99 \
        -Wno-error=char-subscripts\
        -Wno-error=missing-braces \
        -ffunction-sections -fdata-sections -fno-strict-aliasing \
		-fno-builtin -fshort-enums -Wno-packed-bitfield-compat \
")

include_directories(
  ${NRF52_SDK}/external/zboss/include/zcl
  ${NRF52_SDK}/components/libraries/gpiote
  ${NRF52_SDK}/external/fprintf
  ${NRF52_SDK}/integration/nrfx/legacy
  ${NRF52_SDK}/components/libraries/experimental_section_vars
  ${NRF52_SDK}/external/zboss/osif
  ${NRF52_SDK}/components/libraries/atomic_fifo
  ble_config
  ${NRF52_SDK}/external/nRF-IEEE-802.15.4-radio-driver/src
  ${NRF52_SDK}/external/nRF-IEEE-802.15.4-radio-driver/src/fem/three_pin_gpio
  ${NRF52_SDK}/components/libraries/delay
  ${NRF52_SDK}/external/zboss/include
  ${NRF52_SDK}/components/toolchain/cmsis/include
  ${NRF52_SDK}/components/libraries/balloc
  app_utils/ws2812
  ${NRF52_SDK}/components/libraries/log
  ${NRF52_SDK}/components/libraries/memobj
  ${NRF52_SDK}/components/libraries/atomic
  ${NRF52_SDK}/components
  ${NRF52_SDK}/modules/nrfx/mdk
  ${NRF52_SDK}/components/libraries/scheduler
  ${NRF52_SDK}/components/libraries/strerror
  ${NRF52_SDK}/integration/nrfx
  ${NRF52_SDK}/modules/nrfx/drivers/include
  ${NRF52_SDK}/components/zigbee/common
  ${NRF52_SDK}/components/libraries/pwm
  ${NRF52_SDK}/components/libraries/ringbuf
  ${NRF52_SDK}/external/zboss/include/ha
  ${NRF52_SDK}/modules/nrfx
  ${NRF52_SDK}/components/drivers_nrf/nrf_soc_nosd
  ${NRF52_SDK}/components/libraries/log/src
  ${NRF52_SDK}/external/zboss/addons
  ${NRF52_SDK}/external/zboss/include/osif
  ${NRF52_SDK}/components/libraries/sortlist
  ${NRF52_SDK}/external/zboss/zb_error
  ${NRF52_SDK}/modules/nrfx/hal
  ${NRF52_SDK}/components/libraries/mutex
  ${NRF52_SDK}/components/libraries/queue
  ${NRF52_SDK}/components/libraries/pwr_mgmt
  ${NRF52_SDK}/components/libraries/bsp
  ${NRF52_SDK}/components/libraries/fstorage
  ${NRF52_SDK}/components/boards
  ${NRF52_SDK}/components/libraries/timer
  ${NRF52_SDK}/components/libraries/button
  ${NRF52_SDK}/external/nRF-IEEE-802.15.4-radio-driver/src/fem
  ${NRF52_SDK}/components/libraries/util
  $(NRF52_SDK)/components/softdevice/mbr/headers
  $(NRF52_SDK)/external/zboss/lib/gcc
  $(NRF52_SDK)/external/zboss/lib/gcc/nrf52840
)

set(CMAKE_EXE_LINKER_FLAGS "${ARCH_FLAGS} \
        --specs=nano.specs \
        -T${PROJECT_SOURCE_DIR}/blinky.ld \
        -Wl,--gc-sections \
        -L${NRF52_SDK}/modules/nrfx/mdk \
        -L${NRF52_SDK}/external/zboss/lib/gcc \
        -L${NRF52_SDK}/external/zboss/lib/gcc/nrf52840 \
        -lc \
        -lnosys \
        -lm \
        -lstdc++ \
")

set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <LINK_FLAGS> <OBJECTS> -o <TARGET>")

add_executable(light_bulb

  ${NRF52_SDK}/modules/nrfx/mdk/gcc_startup_nrf52840.S
  
  ${NRF52_SDK}/components/libraries/log/src/nrf_log_backend_serial.c
  ${NRF52_SDK}/components/libraries/log/src/nrf_log_backend_uart.c
  ${NRF52_SDK}/components/libraries/log/src/nrf_log_default_backends.c
  ${NRF52_SDK}/components/libraries/log/src/nrf_log_frontend.c
  ${NRF52_SDK}/components/libraries/log/src/nrf_log_str_formatter.c
  ${NRF52_SDK}/components/boards/boards.c
  ${NRF52_SDK}/external/zboss/zb_error/zb_error_to_string.c
  ${NRF52_SDK}/external/zboss/osif/zb_nrf52_common.c
  ${NRF52_SDK}/external/zboss/osif/zb_nrf52_nrf_logger.c
  ${NRF52_SDK}/external/zboss/osif/zb_nrf52_nvram.c
  ${NRF52_SDK}/external/zboss/osif/zb_nrf52_sdk_config_deps.c
  ${NRF52_SDK}/external/zboss/osif/zb_nrf52_timer.c
  ${NRF52_SDK}/external/zboss/osif/zb_nrf52_transceiver.c
  ${NRF52_SDK}/external/zboss/addons/zcl/zb_zcl_common_addons.c
  ${NRF52_SDK}/external/zboss/addons/zcl/zb_zcl_ota_upgrade_addons.c
  ${NRF52_SDK}/components/libraries/button/app_button.c
  ${NRF52_SDK}/components/libraries/util/app_error.c
  ${NRF52_SDK}/components/libraries/util/app_error_handler_gcc.c
  ${NRF52_SDK}/components/libraries/gpiote/app_gpiote.c
  ${NRF52_SDK}/components/libraries/pwm/app_pwm.c
  ${NRF52_SDK}/components/libraries/scheduler/app_scheduler.c
  ${NRF52_SDK}/components/libraries/timer/app_timer2.c
  ${NRF52_SDK}/components/libraries/util/app_util_platform.c
  ${NRF52_SDK}/components/libraries/assert/assert.c
  ${NRF52_SDK}/components/libraries/timer/drv_rtc.c
  ${NRF52_SDK}/components/libraries/util/nrf_assert.c
  ${NRF52_SDK}/components/libraries/atomic_fifo/nrf_atfifo.c
  ${NRF52_SDK}/components/libraries/atomic/nrf_atomic.c
  ${NRF52_SDK}/components/libraries/balloc/nrf_balloc.c
  ${NRF52_SDK}/external/fprintf/nrf_fprintf.c
  ${NRF52_SDK}/external/fprintf/nrf_fprintf_format.c
  ${NRF52_SDK}/components/libraries/fstorage/nrf_fstorage.c
  ${NRF52_SDK}/components/libraries/fstorage/nrf_fstorage_nvmc.c
  ${NRF52_SDK}/components/libraries/memobj/nrf_memobj.c
  ${NRF52_SDK}/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c
  ${NRF52_SDK}/components/libraries/queue/nrf_queue.c
  ${NRF52_SDK}/components/libraries/ringbuf/nrf_ringbuf.c
  ${NRF52_SDK}/components/libraries/experimental_section_vars/nrf_section_iter.c
  ${NRF52_SDK}/components/libraries/sortlist/nrf_sortlist.c
  ${NRF52_SDK}/components/libraries/strerror/nrf_strerror.c
  ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_clock.c
  ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_ppi.c
  ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_rng.c
  ${NRF52_SDK}/integration/nrfx/legacy/nrf_drv_uart.c
  ${NRF52_SDK}/modules/nrfx/hal/nrf_ecb.c
  ${NRF52_SDK}/components/drivers_nrf/nrf_soc_nosd/nrf_nvic.c
  ${NRF52_SDK}/modules/nrfx/hal/nrf_nvmc.c
  ${NRF52_SDK}/components/drivers_nrf/nrf_soc_nosd/nrf_soc.c
  ${NRF52_SDK}/modules/nrfx/soc/nrfx_atomic.c
  ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_clock.c
  ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_gpiote.c
  ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_ppi.c
  ${NRF52_SDK}/modules/nrfx/drivers/src/prs/nrfx_prs.c
  ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_pwm.c
  ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_rng.c
  ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_systick.c
  ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_timer.c
  ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_uart.c
  ${NRF52_SDK}/modules/nrfx/drivers/src/nrfx_uarte.c
  ${NRF52_SDK}/components/libraries/bsp/bsp.c
  light_bulb.c
  ${NRF52_SDK}/modules/nrfx/mdk/system_nrf52840.c
  ${NRF52_SDK}/components/zigbee/common/zigbee_helpers.c
  ${NRF52_SDK}/components/zigbee/common/zigbee_logger_eprxzcl.c
  app_utils/ws2812/drv_ws2812.c
)

ADD_LIBRARY(zboss STATIC IMPORTED)
SET_TARGET_PROPERTIES(zboss PROPERTIES IMPORTED_LOCATION /opt/nRF5_SDK_for_Thread_and_Zigbee_v4.0.0_dc7186b/external/zboss/lib/gcc/libzboss.a)
TARGET_LINK_LIBRARIES(light_bulb A zboss C)


add_custom_command(
        OUTPUT ${SELECTED_TARGET}.hex
        COMMAND arm-none-eabi-objcopy -O ihex ${SELECTED_TARGET} ${SELECTED_TARGET}.hex
        DEPENDS ${SELECTED_TARGET}
)

add_custom_target(
        hex
        DEPENDS ${SELECTED_TARGET}.hex
)

add_custom_target(
        flash_openocd
        DEPENDS ${SELECTED_TARGET}.hex
        COMMAND ${OPENOCD_BINARY} -f ${CMAKE_SOURCE_DIR}/openocd.cfg -c \"init\; reset halt\; nrf51 mass_erase\; program /home/seregia/Documents/projects/mcu/nrf52-zigbee-test/build/Default/${SELECTED_TARGET}.hex verify reset\; exit\;\"
        )

