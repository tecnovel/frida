################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/class/dfu/dfu_device.c \
../Core/TinyUSB/class/dfu/dfu_rt_device.c 

OBJS += \
./Core/TinyUSB/class/dfu/dfu_device.o \
./Core/TinyUSB/class/dfu/dfu_rt_device.o 

C_DEPS += \
./Core/TinyUSB/class/dfu/dfu_device.d \
./Core/TinyUSB/class/dfu/dfu_rt_device.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/class/dfu/%.o Core/TinyUSB/class/dfu/%.su Core/TinyUSB/class/dfu/%.cyclo: ../Core/TinyUSB/class/dfu/%.c Core/TinyUSB/class/dfu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-class-2f-dfu

clean-Core-2f-TinyUSB-2f-class-2f-dfu:
	-$(RM) ./Core/TinyUSB/class/dfu/dfu_device.cyclo ./Core/TinyUSB/class/dfu/dfu_device.d ./Core/TinyUSB/class/dfu/dfu_device.o ./Core/TinyUSB/class/dfu/dfu_device.su ./Core/TinyUSB/class/dfu/dfu_rt_device.cyclo ./Core/TinyUSB/class/dfu/dfu_rt_device.d ./Core/TinyUSB/class/dfu/dfu_rt_device.o ./Core/TinyUSB/class/dfu/dfu_rt_device.su

.PHONY: clean-Core-2f-TinyUSB-2f-class-2f-dfu

