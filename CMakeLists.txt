cmake_minimum_required(VERSION 3.5.1)
project(blinky C ASM)

set(CMAKE_VERBOSE_MAKEFILE 1)

set(NRF52_SDK "/home/wieker/Projects/linux/nrf/nRF5_SDK_for_Thread_and_Zigbee_v4.0.0_dc7186b")
# or
# -DNRF52_SDK=...

set(OPENOCD_BINARY "/home/wieker/Projects/linux/hackrf/openocd-bin/bin/openocd")
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
        -lzboss \
        -lzboss.ed \
        /home/wieker/Projects/aaa/af_descriptor.o \
        /home/wieker/Projects/aaa/af_interpan.o \
        /home/wieker/Projects/aaa/af_rx.o \
        /home/wieker/Projects/aaa/aps_bind.o \
        /home/wieker/Projects/aaa/aps_commands.o \
        /home/wieker/Projects/aaa/aps_dups.o \
        /home/wieker/Projects/aaa/aps_interpan.o \
        /home/wieker/Projects/aaa/aps_main.o \
        /home/wieker/Projects/aaa/apsme_secur.o \
        /home/wieker/Projects/aaa/aps_nwk_confirm.o \
        /home/wieker/Projects/aaa/aps_secur.o \
        /home/wieker/Projects/aaa/bdb_commissioning.o \
        /home/wieker/Projects/aaa/bdb_finding_binding.o \
        /home/wieker/Projects/aaa/bdb_secur.o \
        /home/wieker/Projects/aaa/ha_sas.o \
        /home/wieker/Projects/aaa/ic_secur.o \
        /home/wieker/Projects/aaa/joining_list_mgmt.o \
        /home/wieker/Projects/aaa/mac_api_trace.o \
        /home/wieker/Projects/aaa/mac_associate.o \
        /home/wieker/Projects/aaa/mac_cb_stubs.o \
        /home/wieker/Projects/aaa/mac_common.o \
        /home/wieker/Projects/aaa/mac_cr_associate.o \
        /home/wieker/Projects/aaa/mac_cr_coordinator.o \
        /home/wieker/Projects/aaa/mac_cr_data.o \
        /home/wieker/Projects/aaa/mac_data.o \
        /home/wieker/Projects/aaa/mac_disturber.o \
        /home/wieker/Projects/aaa/mac_dump.o \
        /home/wieker/Projects/aaa/mac_duty_cycle.o \
        /home/wieker/Projects/aaa/mac_fcs.o \
        /home/wieker/Projects/aaa/mac_nrf52840_driver.o \
        /home/wieker/Projects/aaa/mac_nrf52840_mac_cert_stubs.o \
        /home/wieker/Projects/aaa/mac.o \
        /home/wieker/Projects/aaa/mac_optional.o \
        /home/wieker/Projects/aaa/mac_pib.o \
        /home/wieker/Projects/aaa/mac_scan.o \
        /home/wieker/Projects/aaa/mac_source_matching.o \
        /home/wieker/Projects/aaa/mac_visibility.o \
        /home/wieker/Projects/aaa/mac_zcl_diagnostic.o \
        /home/wieker/Projects/aaa/mac_zgp.o \
        /home/wieker/Projects/aaa/nwk_address_assign.o \
        /home/wieker/Projects/aaa/nwk_address_conflict.o \
        /home/wieker/Projects/aaa/nwk_broadcasting.o \
        /home/wieker/Projects/aaa/nwk_cr_formation.o \
        /home/wieker/Projects/aaa/nwk_cr_join.o \
        /home/wieker/Projects/aaa/nwk_cr_mesh_routing.o \
        /home/wieker/Projects/aaa/nwk_cr_parent.o \
        /home/wieker/Projects/aaa/nwk_cr_permit_join.o \
        /home/wieker/Projects/aaa/nwk_cr_route_discovery.o \
        /home/wieker/Projects/aaa/nwk_cr_tree_routing.o \
        /home/wieker/Projects/aaa/nwk_discovery.o \
        /home/wieker/Projects/aaa/nwk_formation.o \
        /home/wieker/Projects/aaa/nwk_join.o \
        /home/wieker/Projects/aaa/nwk_link_power_delta.o \
        /home/wieker/Projects/aaa/nwk_link_status.o \
        /home/wieker/Projects/aaa/nwk_main.o \
        /home/wieker/Projects/aaa/nwk_mm.o \
        /home/wieker/Projects/aaa/nwk_neighbor.o \
        /home/wieker/Projects/aaa/nwk_neighbor_utility.o \
        /home/wieker/Projects/aaa/nwk_nlme.o \
        /home/wieker/Projects/aaa/nwk_panid_conflict.o \
        /home/wieker/Projects/aaa/nwk_secur.o \
        /home/wieker/Projects/aaa/nwk_srouting.o \
        /home/wieker/Projects/aaa/secur_ccm.o \
        /home/wieker/Projects/aaa/secur_formation.o \
        /home/wieker/Projects/aaa/test_profile.o \
        /home/wieker/Projects/aaa/zb_48bit_math.o \
        /home/wieker/Projects/aaa/zb_address.o \
        /home/wieker/Projects/aaa/zb_bufpool_mult.o \
        /home/wieker/Projects/aaa/zb_bufpool_mult_storage.o \
        /home/wieker/Projects/aaa/zb_channel_page.o \
        /home/wieker/Projects/aaa/zb_debug.o \
        /home/wieker/Projects/aaa/zb_ecc.o \
        /home/wieker/Projects/aaa/zb_error_indication.o \
        /home/wieker/Projects/aaa/zb_fsm.o \
        /home/wieker/Projects/aaa/zb_ie.o \
        /home/wieker/Projects/aaa/zb_init.o \
        /home/wieker/Projects/aaa/zb_nrf52_spi_slave_ncp.o \
        /home/wieker/Projects/aaa/zb_nrf52_spi_slave.o \
        /home/wieker/Projects/aaa/zb_nrf52_zboss_deps.o \
        /home/wieker/Projects/aaa/zb_nvram.o \
        /home/wieker/Projects/aaa/zb_nwk_ed_aging.o \
        /home/wieker/Projects/aaa/zb_random.o \
        /home/wieker/Projects/aaa/zb_scheduler_init.o \
        /home/wieker/Projects/aaa/zb_scheduler.o \
        /home/wieker/Projects/aaa/zb_serial_trace_bin.o \
        /home/wieker/Projects/aaa/zb_sleep.o \
        /home/wieker/Projects/aaa/zb_time.o \
        /home/wieker/Projects/aaa/zb_trace_common.o \
        /home/wieker/Projects/aaa/zb_watchdog.o \
        /home/wieker/Projects/aaa/zb_zboss_api.o \
        /home/wieker/Projects/aaa/zcl_alarms_commands.o \
        /home/wieker/Projects/aaa/zcl_attr_value.o \
        /home/wieker/Projects/aaa/zcl_basic_commands.o \
        /home/wieker/Projects/aaa/zcl_binary_input.o \
        /home/wieker/Projects/aaa/zcl_c_drlc.o \
        /home/wieker/Projects/aaa/zcl_c_messaging.o \
        /home/wieker/Projects/aaa/zcl_c_metering.o \
        /home/wieker/Projects/aaa/zcl_color_control_commands.o \
        /home/wieker/Projects/aaa/zcl_common.o \
        /home/wieker/Projects/aaa/zcl_continuous_value_change_commands.o \
        /home/wieker/Projects/aaa/zcl_c_price.o \
        /home/wieker/Projects/aaa/zcl_c_tunneling.o \
        /home/wieker/Projects/aaa/zcl_custom_cluster_commands.o \
        /home/wieker/Projects/aaa/zcl_c_wwah.o \
        /home/wieker/Projects/aaa/zcl_dehumid_control.o \
        /home/wieker/Projects/aaa/zcl_diagnostics_commands.o \
        /home/wieker/Projects/aaa/zcl_door_lock.o \
        /home/wieker/Projects/aaa/zcl_el_measurement.o \
        /home/wieker/Projects/aaa/zcl_en50523_appliance_events_and_alerts.o \
        /home/wieker/Projects/aaa/zcl_fan_control.o \
        /home/wieker/Projects/aaa/zcl_general_commands.o \
        /home/wieker/Projects/aaa/zcl_groups.o \
        /home/wieker/Projects/aaa/zcl_ias_ace_commands.o \
        /home/wieker/Projects/aaa/zcl_ias_wd_commands.o \
        /home/wieker/Projects/aaa/zcl_ias_zone_commands.o \
        /home/wieker/Projects/aaa/zcl_identify_commands.o \
        /home/wieker/Projects/aaa/zcl_illuminance_measurement.o \
        /home/wieker/Projects/aaa/zcl_level_control_commands.o \
        /home/wieker/Projects/aaa/zcl_main.o \
        /home/wieker/Projects/aaa/zcl_meter_identification.o \
        /home/wieker/Projects/aaa/zcl_on_off_commands.o \
        /home/wieker/Projects/aaa/zcl_on_off_switch_config.o \
        /home/wieker/Projects/aaa/zcl_ota_upgrade_commands.o \
        /home/wieker/Projects/aaa/zcl_ota_upgrade_common.o \
        /home/wieker/Projects/aaa/zcl_ota_upgrade_minimal.o \
        /home/wieker/Projects/aaa/zcl_ota_upgrade_srv_commands.o \
        /home/wieker/Projects/aaa/zcl_poll_control_client.o \
        /home/wieker/Projects/aaa/zcl_poll_control_commands.o \
        /home/wieker/Projects/aaa/zcl_power_config_commands.o \
        /home/wieker/Projects/aaa/zcl_rel_humidity.o \
        /home/wieker/Projects/aaa/zcl_reporting.o \
        /home/wieker/Projects/aaa/zcl_scenes.o \
        /home/wieker/Projects/aaa/zcl_s_drlc.o \
        /home/wieker/Projects/aaa/zcl_shade_config_commands.o \
        /home/wieker/Projects/aaa/zcl_s_messaging.o \
        /home/wieker/Projects/aaa/zcl_s_metering.o \
        /home/wieker/Projects/aaa/zcl_s_price.o \
        /home/wieker/Projects/aaa/zcl_s_tunneling.o \
        /home/wieker/Projects/aaa/zcl_s_wwah.o \
        /home/wieker/Projects/aaa/zcl_temp_measurement.o \
        /home/wieker/Projects/aaa/zcl_thermostat_commands.o \
        /home/wieker/Projects/aaa/zcl_thermostat_ui_config.o \
        /home/wieker/Projects/aaa/zcl_time.o \
        /home/wieker/Projects/aaa/zcl_window_covering.o \
        /home/wieker/Projects/aaa/zcl_wwah_common.o \
        /home/wieker/Projects/aaa/zdo_app.o \
        /home/wieker/Projects/aaa/zdo_bind_manage.o \
        /home/wieker/Projects/aaa/zdo_channel_manager.o \
        /home/wieker/Projects/aaa/zdo_commissioning_bdb.o \
        /home/wieker/Projects/aaa/zdo_commissioning_classic.o \
        /home/wieker/Projects/aaa/zdo_commissioning.o \
        /home/wieker/Projects/aaa/zdo_common.o \
        /home/wieker/Projects/aaa/zdo_disc_cli.o \
        /home/wieker/Projects/aaa/zdo_disc_srv.o \
        /home/wieker/Projects/aaa/zdo_ed_aging.o \
        /home/wieker/Projects/aaa/zdo_formation.o \
        /home/wieker/Projects/aaa/zdo_nwk_manage_cli.o \
        /home/wieker/Projects/aaa/zdo_nwk_manage_srv.o \
        /home/wieker/Projects/aaa/zdo_poll.o \
        /home/wieker/Projects/aaa/zdo_promisc.o \
        /home/wieker/Projects/aaa/zdo_resp_validate.o \
        /home/wieker/Projects/aaa/zdo_rx.o \
        /home/wieker/Projects/aaa/zdo_secur.o \
        /home/wieker/Projects/aaa/zdo_wwah_parent_classification.o \
        /home/wieker/Projects/aaa/zdo_wwah_survey_beacons.o \
        /home/wieker/Projects/aaa/zgp_cluster_gp.o \
        /home/wieker/Projects/aaa/zgp_cluster.o \
        /home/wieker/Projects/aaa/zgp_common.o \
        /home/wieker/Projects/aaa/zgp_helper.o \
        /home/wieker/Projects/aaa/zgp_proxy_table.o \
        /home/wieker/Projects/aaa/zgp_secur.o \
        /home/wieker/Projects/aaa/zgp_sink.o \
        /home/wieker/Projects/aaa/zgp_stub.o \
        /home/wieker/Projects/aaa/zgp_tx_queue.o \
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
  ${NRF52_SDK}/external/nRF-IEEE-802.15.4-radio-driver/src/nrf_802154.c
  light_bulb.c
  ${NRF52_SDK}/modules/nrfx/mdk/system_nrf52840.c
  ${NRF52_SDK}/components/zigbee/common/zigbee_helpers.c
  ${NRF52_SDK}/components/zigbee/common/zigbee_logger_eprxzcl.c
  app_utils/ws2812/drv_ws2812.c
  ${NRF52_SDK}/external/nRF-IEEE-802.15.4-radio-driver/src/nrf_802154_pib.c
)

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

