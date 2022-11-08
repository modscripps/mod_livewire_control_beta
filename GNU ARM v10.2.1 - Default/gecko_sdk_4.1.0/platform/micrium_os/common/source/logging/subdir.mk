################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk/platform/micrium_os/common/source/logging/logging.c 

OBJS += \
./gecko_sdk_4.1.0/platform/micrium_os/common/source/logging/logging.o 

C_DEPS += \
./gecko_sdk_4.1.0/platform/micrium_os/common/source/logging/logging.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_4.1.0/platform/micrium_os/common/source/logging/logging.o: /Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk/platform/micrium_os/common/source/logging/logging.c gecko_sdk_4.1.0/platform/micrium_os/common/source/logging/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DDEBUG_EFM=1' '-DEFM32GG11B820F2048GL192=1' '-DSL_BOARD_NAME="BRD2204A"' '-DSL_BOARD_REV="B07"' '-DconfigNUM_SDK_THREAD_LOCAL_STORAGE_POINTERS=2' '-DSL_COMPONENT_CATALOG_PRESENT=1' -I"/Users/aleboyer/SimplicityStudio/v5_workspace/mod_livewire_control_beta/FatFS/inc" -I"/Users/aleboyer/SimplicityStudio/v5_workspace/mod_livewire_control_beta/SDIO_Driver/inc" -I"/Users/aleboyer/SimplicityStudio/v5_workspace/mod_livewire_control_beta/hall effects/inc" -I"/Users/aleboyer/SimplicityStudio/v5_workspace/mod_livewire_control_beta/config" -I"/Users/aleboyer/SimplicityStudio/v5_workspace/mod_livewire_control_beta/autogen" -I"/Users/aleboyer/SimplicityStudio/v5_workspace/mod_livewire_control_beta" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/Device/SiliconLabs/EFM32GG11B/Include" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/common/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//hardware/board/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/driver/button/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/service/cli/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/service/cli/src" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/Core/Include" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/RTOS2/Include" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/service/device_init/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/common/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/emdrv/gpiointerrupt/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/service/iostream/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/driver/leddrv/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/micrium_os/common/source" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/micrium_os/common/include" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/micrium_os/cpu/include" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/micrium_os/ports/source" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/micrium_os" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/micrium_os/kernel/source" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/micrium_os/kernel/include" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/common/toolchain/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/service/system/inc" -I"/Users/aleboyer/SimplicityStudio/SDKs/gecko_sdk//platform/service/sleeptimer/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.1.0/platform/micrium_os/common/source/logging/logging.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


