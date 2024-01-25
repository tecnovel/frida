################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/portable/wch/dcd_ch32_usbhs.c 

OBJS += \
./Core/TinyUSB/portable/wch/dcd_ch32_usbhs.o 

C_DEPS += \
./Core/TinyUSB/portable/wch/dcd_ch32_usbhs.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/portable/wch/%.o Core/TinyUSB/portable/wch/%.su Core/TinyUSB/portable/wch/%.cyclo: ../Core/TinyUSB/portable/wch/%.c Core/TinyUSB/portable/wch/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-portable-2f-wch

clean-Core-2f-TinyUSB-2f-portable-2f-wch:
	-$(RM) ./Core/TinyUSB/portable/wch/dcd_ch32_usbhs.cyclo ./Core/TinyUSB/portable/wch/dcd_ch32_usbhs.d ./Core/TinyUSB/portable/wch/dcd_ch32_usbhs.o ./Core/TinyUSB/portable/wch/dcd_ch32_usbhs.su

.PHONY: clean-Core-2f-TinyUSB-2f-portable-2f-wch

