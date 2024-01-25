################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/portable/renesas/rusb2/dcd_rusb2.c \
../Core/TinyUSB/portable/renesas/rusb2/hcd_rusb2.c \
../Core/TinyUSB/portable/renesas/rusb2/rusb2_common.c 

OBJS += \
./Core/TinyUSB/portable/renesas/rusb2/dcd_rusb2.o \
./Core/TinyUSB/portable/renesas/rusb2/hcd_rusb2.o \
./Core/TinyUSB/portable/renesas/rusb2/rusb2_common.o 

C_DEPS += \
./Core/TinyUSB/portable/renesas/rusb2/dcd_rusb2.d \
./Core/TinyUSB/portable/renesas/rusb2/hcd_rusb2.d \
./Core/TinyUSB/portable/renesas/rusb2/rusb2_common.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/portable/renesas/rusb2/%.o Core/TinyUSB/portable/renesas/rusb2/%.su Core/TinyUSB/portable/renesas/rusb2/%.cyclo: ../Core/TinyUSB/portable/renesas/rusb2/%.c Core/TinyUSB/portable/renesas/rusb2/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-portable-2f-renesas-2f-rusb2

clean-Core-2f-TinyUSB-2f-portable-2f-renesas-2f-rusb2:
	-$(RM) ./Core/TinyUSB/portable/renesas/rusb2/dcd_rusb2.cyclo ./Core/TinyUSB/portable/renesas/rusb2/dcd_rusb2.d ./Core/TinyUSB/portable/renesas/rusb2/dcd_rusb2.o ./Core/TinyUSB/portable/renesas/rusb2/dcd_rusb2.su ./Core/TinyUSB/portable/renesas/rusb2/hcd_rusb2.cyclo ./Core/TinyUSB/portable/renesas/rusb2/hcd_rusb2.d ./Core/TinyUSB/portable/renesas/rusb2/hcd_rusb2.o ./Core/TinyUSB/portable/renesas/rusb2/hcd_rusb2.su ./Core/TinyUSB/portable/renesas/rusb2/rusb2_common.cyclo ./Core/TinyUSB/portable/renesas/rusb2/rusb2_common.d ./Core/TinyUSB/portable/renesas/rusb2/rusb2_common.o ./Core/TinyUSB/portable/renesas/rusb2/rusb2_common.su

.PHONY: clean-Core-2f-TinyUSB-2f-portable-2f-renesas-2f-rusb2

