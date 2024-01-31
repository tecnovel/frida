################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/portable/nordic/nrf5x/dcd_nrf5x.c 

OBJS += \
./Core/TinyUSB/portable/nordic/nrf5x/dcd_nrf5x.o 

C_DEPS += \
./Core/TinyUSB/portable/nordic/nrf5x/dcd_nrf5x.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/portable/nordic/nrf5x/%.o Core/TinyUSB/portable/nordic/nrf5x/%.su Core/TinyUSB/portable/nordic/nrf5x/%.cyclo: ../Core/TinyUSB/portable/nordic/nrf5x/%.c Core/TinyUSB/portable/nordic/nrf5x/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -I../FATFS/Target -I../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-portable-2f-nordic-2f-nrf5x

clean-Core-2f-TinyUSB-2f-portable-2f-nordic-2f-nrf5x:
	-$(RM) ./Core/TinyUSB/portable/nordic/nrf5x/dcd_nrf5x.cyclo ./Core/TinyUSB/portable/nordic/nrf5x/dcd_nrf5x.d ./Core/TinyUSB/portable/nordic/nrf5x/dcd_nrf5x.o ./Core/TinyUSB/portable/nordic/nrf5x/dcd_nrf5x.su

.PHONY: clean-Core-2f-TinyUSB-2f-portable-2f-nordic-2f-nrf5x

