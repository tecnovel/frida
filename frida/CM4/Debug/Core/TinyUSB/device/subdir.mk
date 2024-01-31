################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/device/usbd.c \
../Core/TinyUSB/device/usbd_control.c 

OBJS += \
./Core/TinyUSB/device/usbd.o \
./Core/TinyUSB/device/usbd_control.o 

C_DEPS += \
./Core/TinyUSB/device/usbd.d \
./Core/TinyUSB/device/usbd_control.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/device/%.o Core/TinyUSB/device/%.su Core/TinyUSB/device/%.cyclo: ../Core/TinyUSB/device/%.c Core/TinyUSB/device/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-device

clean-Core-2f-TinyUSB-2f-device:
	-$(RM) ./Core/TinyUSB/device/usbd.cyclo ./Core/TinyUSB/device/usbd.d ./Core/TinyUSB/device/usbd.o ./Core/TinyUSB/device/usbd.su ./Core/TinyUSB/device/usbd_control.cyclo ./Core/TinyUSB/device/usbd_control.d ./Core/TinyUSB/device/usbd_control.o ./Core/TinyUSB/device/usbd_control.su

.PHONY: clean-Core-2f-TinyUSB-2f-device

