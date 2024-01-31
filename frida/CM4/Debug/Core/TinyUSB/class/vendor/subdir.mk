################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/class/vendor/vendor_device.c \
../Core/TinyUSB/class/vendor/vendor_host.c 

OBJS += \
./Core/TinyUSB/class/vendor/vendor_device.o \
./Core/TinyUSB/class/vendor/vendor_host.o 

C_DEPS += \
./Core/TinyUSB/class/vendor/vendor_device.d \
./Core/TinyUSB/class/vendor/vendor_host.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/class/vendor/%.o Core/TinyUSB/class/vendor/%.su Core/TinyUSB/class/vendor/%.cyclo: ../Core/TinyUSB/class/vendor/%.c Core/TinyUSB/class/vendor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-class-2f-vendor

clean-Core-2f-TinyUSB-2f-class-2f-vendor:
	-$(RM) ./Core/TinyUSB/class/vendor/vendor_device.cyclo ./Core/TinyUSB/class/vendor/vendor_device.d ./Core/TinyUSB/class/vendor/vendor_device.o ./Core/TinyUSB/class/vendor/vendor_device.su ./Core/TinyUSB/class/vendor/vendor_host.cyclo ./Core/TinyUSB/class/vendor/vendor_host.d ./Core/TinyUSB/class/vendor/vendor_host.o ./Core/TinyUSB/class/vendor/vendor_host.su

.PHONY: clean-Core-2f-TinyUSB-2f-class-2f-vendor

