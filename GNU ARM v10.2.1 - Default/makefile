################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include hall\ effects/src/subdir.mk
-include gecko_sdk_4.1.0/platform/service/system/src/subdir.mk
-include gecko_sdk_4.1.0/platform/service/sleeptimer/src/subdir.mk
-include gecko_sdk_4.1.0/platform/service/iostream/src/subdir.mk
-include gecko_sdk_4.1.0/platform/service/device_init/src/subdir.mk
-include gecko_sdk_4.1.0/platform/service/cli/src/subdir.mk
-include gecko_sdk_4.1.0/platform/micrium_os/ports/source/gnu/subdir.mk
-include gecko_sdk_4.1.0/platform/micrium_os/ports/source/generic/subdir.mk
-include gecko_sdk_4.1.0/platform/micrium_os/kernel/source/subdir.mk
-include gecko_sdk_4.1.0/platform/micrium_os/cpu/source/subdir.mk
-include gecko_sdk_4.1.0/platform/micrium_os/common/source/rtos/subdir.mk
-include gecko_sdk_4.1.0/platform/micrium_os/common/source/ring_buf/subdir.mk
-include gecko_sdk_4.1.0/platform/micrium_os/common/source/logging/subdir.mk
-include gecko_sdk_4.1.0/platform/micrium_os/common/source/lib/subdir.mk
-include gecko_sdk_4.1.0/platform/micrium_os/common/source/kal/subdir.mk
-include gecko_sdk_4.1.0/platform/emlib/src/subdir.mk
-include gecko_sdk_4.1.0/platform/emdrv/gpiointerrupt/src/subdir.mk
-include gecko_sdk_4.1.0/platform/driver/leddrv/src/subdir.mk
-include gecko_sdk_4.1.0/platform/driver/button/src/subdir.mk
-include gecko_sdk_4.1.0/platform/common/toolchain/src/subdir.mk
-include gecko_sdk_4.1.0/platform/common/src/subdir.mk
-include gecko_sdk_4.1.0/platform/Device/SiliconLabs/EFM32GG11B/Source/subdir.mk
-include gecko_sdk_4.1.0/platform/CMSIS/RTOS2/Source/subdir.mk
-include gecko_sdk_4.1.0/hardware/board/src/subdir.mk
-include emlib/subdir.mk
-include autogen/subdir.mk
-include SDIO_Driver/src/subdir.mk
-include FatFS/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := mod_livewire_control_beta
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: mod_livewire_control_beta.axf

# Tool invocations
mod_livewire_control_beta.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "/Users/aleboyer/SimplicityStudio/v5_workspace/mod_livewire_control_beta/autogen/linkerfile.ld" -Xlinker --gc-sections -Xlinker -Map="mod_livewire_control_beta.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o mod_livewire_control_beta.axf -Wl,--start-group "./FatFS/src/diskio.o" "./FatFS/src/ff.o" "./FatFS/src/ffsystem.o" "./FatFS/src/ffunicode.o" "./SDIO_Driver/src/sdio.o" "./app.o" "./cli.o" "./main.o" "./mod_fatfs_chan.o" "./mod_utilities.o" "./sbe.o" "./task.o" "./autogen/sl_board_default_init.o" "./autogen/sl_cli_command_table.o" "./autogen/sl_cli_instances.o" "./autogen/sl_device_init_clocks.o" "./autogen/sl_event_handler.o" "./autogen/sl_iostream_handles.o" "./autogen/sl_iostream_init_usart_instances.o" "./autogen/sl_simple_button_instances.o" "./autogen/sl_simple_led_instances.o" "./autogen/sli_cli_hooks.o" "./emlib/em_adc.o" "./gecko_sdk_4.1.0/hardware/board/src/sl_board_control_gpio.o" "./gecko_sdk_4.1.0/hardware/board/src/sl_board_init.o" "./gecko_sdk_4.1.0/platform/CMSIS/RTOS2/Source/os_systick.o" "./gecko_sdk_4.1.0/platform/Device/SiliconLabs/EFM32GG11B/Source/startup_efm32gg11b.o" "./gecko_sdk_4.1.0/platform/Device/SiliconLabs/EFM32GG11B/Source/system_efm32gg11b.o" "./gecko_sdk_4.1.0/platform/common/src/sl_assert.o" "./gecko_sdk_4.1.0/platform/common/src/sl_slist.o" "./gecko_sdk_4.1.0/platform/common/src/sl_string.o" "./gecko_sdk_4.1.0/platform/common/src/sli_cmsis_os2_ext_task_register.o" "./gecko_sdk_4.1.0/platform/common/toolchain/src/sl_memory.o" "./gecko_sdk_4.1.0/platform/driver/button/src/sl_button.o" "./gecko_sdk_4.1.0/platform/driver/button/src/sl_simple_button.o" "./gecko_sdk_4.1.0/platform/driver/leddrv/src/sl_led.o" "./gecko_sdk_4.1.0/platform/driver/leddrv/src/sl_simple_led.o" "./gecko_sdk_4.1.0/platform/emdrv/gpiointerrupt/src/gpiointerrupt.o" "./gecko_sdk_4.1.0/platform/emlib/src/em_cmu.o" "./gecko_sdk_4.1.0/platform/emlib/src/em_core.o" "./gecko_sdk_4.1.0/platform/emlib/src/em_emu.o" "./gecko_sdk_4.1.0/platform/emlib/src/em_gpio.o" "./gecko_sdk_4.1.0/platform/emlib/src/em_msc.o" "./gecko_sdk_4.1.0/platform/emlib/src/em_rtc.o" "./gecko_sdk_4.1.0/platform/emlib/src/em_rtcc.o" "./gecko_sdk_4.1.0/platform/emlib/src/em_system.o" "./gecko_sdk_4.1.0/platform/emlib/src/em_usart.o" "./gecko_sdk_4.1.0/platform/micrium_os/common/source/kal/kal_kernel.o" "./gecko_sdk_4.1.0/platform/micrium_os/common/source/lib/lib_ascii.o" "./gecko_sdk_4.1.0/platform/micrium_os/common/source/lib/lib_mem.o" "./gecko_sdk_4.1.0/platform/micrium_os/common/source/lib/lib_str.o" "./gecko_sdk_4.1.0/platform/micrium_os/common/source/logging/logging.o" "./gecko_sdk_4.1.0/platform/micrium_os/common/source/ring_buf/ring_buf.o" "./gecko_sdk_4.1.0/platform/micrium_os/common/source/rtos/rtos_err_str.o" "./gecko_sdk_4.1.0/platform/micrium_os/common/source/rtos/rtos_utils.o" "./gecko_sdk_4.1.0/platform/micrium_os/cpu/source/cpu_core.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/cmsis_os2.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_cfg_app.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_core.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_dbg.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_flag.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_mon.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_msg.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_mutex.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_prio.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_q.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_sem.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_stat.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_task.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_time.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_tmr.o" "./gecko_sdk_4.1.0/platform/micrium_os/kernel/source/os_var.o" "./gecko_sdk_4.1.0/platform/micrium_os/ports/source/generic/arm_cpu_dwt_ts.o" "./gecko_sdk_4.1.0/platform/micrium_os/ports/source/gnu/armv7m_cpu_a.o" "./gecko_sdk_4.1.0/platform/micrium_os/ports/source/gnu/armv7m_os_cpu_a.o" "./gecko_sdk_4.1.0/platform/micrium_os/ports/source/gnu/armv7m_os_cpu_c.o" "./gecko_sdk_4.1.0/platform/service/cli/src/sl_cli.o" "./gecko_sdk_4.1.0/platform/service/cli/src/sl_cli_arguments.o" "./gecko_sdk_4.1.0/platform/service/cli/src/sl_cli_command.o" "./gecko_sdk_4.1.0/platform/service/cli/src/sl_cli_input.o" "./gecko_sdk_4.1.0/platform/service/cli/src/sl_cli_io.o" "./gecko_sdk_4.1.0/platform/service/cli/src/sl_cli_tokenize.o" "./gecko_sdk_4.1.0/platform/service/device_init/src/sl_device_init_dcdc_s1.o" "./gecko_sdk_4.1.0/platform/service/device_init/src/sl_device_init_emu_s1.o" "./gecko_sdk_4.1.0/platform/service/device_init/src/sl_device_init_hfxo_s1.o" "./gecko_sdk_4.1.0/platform/service/device_init/src/sl_device_init_lfxo_s1.o" "./gecko_sdk_4.1.0/platform/service/device_init/src/sl_device_init_nvic.o" "./gecko_sdk_4.1.0/platform/service/iostream/src/sl_iostream.o" "./gecko_sdk_4.1.0/platform/service/iostream/src/sl_iostream_retarget_stdio.o" "./gecko_sdk_4.1.0/platform/service/iostream/src/sl_iostream_stdlib_config.o" "./gecko_sdk_4.1.0/platform/service/iostream/src/sl_iostream_uart.o" "./gecko_sdk_4.1.0/platform/service/iostream/src/sl_iostream_usart.o" "./gecko_sdk_4.1.0/platform/service/sleeptimer/src/sl_sleeptimer.o" "./gecko_sdk_4.1.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtc.o" "./gecko_sdk_4.1.0/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtcc.o" "./gecko_sdk_4.1.0/platform/service/system/src/sl_system_init.o" "./gecko_sdk_4.1.0/platform/service/system/src/sl_system_kernel.o" "./hall effects/src/hall_effects.o" "./hall effects/src/simple_button_kernel.o" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: mod_livewire_control_beta.hex'
	arm-none-eabi-objcopy -O ihex "mod_livewire_control_beta.axf" "mod_livewire_control_beta.hex"
	@echo ' '

	@echo 'Building bin file: mod_livewire_control_beta.bin'
	arm-none-eabi-objcopy -O binary "mod_livewire_control_beta.axf" "mod_livewire_control_beta.bin"
	@echo ' '

	@echo 'Building s37 file: mod_livewire_control_beta.s37'
	arm-none-eabi-objcopy -O srec "mod_livewire_control_beta.axf" "mod_livewire_control_beta.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "mod_livewire_control_beta.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) mod_livewire_control_beta.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
