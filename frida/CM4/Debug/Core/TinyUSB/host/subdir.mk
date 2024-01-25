################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/host/hub.c \
../Core/TinyUSB/host/usbh.c 

OBJS += \
./Core/TinyUSB/host/hub.o \
./Core/TinyUSB/host/usbh.o 

C_DEPS += \
./Core/TinyUSB/host/hub.d \
./Core/TinyUSB/host/usbh.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/host/%.o Core/TinyUSB/host/%.su Core/TinyUSB/host/%.cyclo: ../Core/TinyUSB/host/%.c Core/TinyUSB/host/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-host

clean-Core-2f-TinyUSB-2f-host:
	-$(RM) ./Core/TinyUSB/host/hub.cyclo ./Core/TinyUSB/host/hub.d ./Core/TinyUSB/host/hub.o ./Core/TinyUSB/host/hub.su ./Core/TinyUSB/host/usbh.cyclo ./Core/TinyUSB/host/usbh.d ./Core/TinyUSB/host/usbh.o ./Core/TinyUSB/host/usbh.su

.PHONY: clean-Core-2f-TinyUSB-2f-host

