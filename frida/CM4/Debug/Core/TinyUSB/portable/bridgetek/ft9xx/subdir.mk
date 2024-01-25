################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.c 

OBJS += \
./Core/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.o 

C_DEPS += \
./Core/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/portable/bridgetek/ft9xx/%.o Core/TinyUSB/portable/bridgetek/ft9xx/%.su Core/TinyUSB/portable/bridgetek/ft9xx/%.cyclo: ../Core/TinyUSB/portable/bridgetek/ft9xx/%.c Core/TinyUSB/portable/bridgetek/ft9xx/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-portable-2f-bridgetek-2f-ft9xx

clean-Core-2f-TinyUSB-2f-portable-2f-bridgetek-2f-ft9xx:
	-$(RM) ./Core/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.cyclo ./Core/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.d ./Core/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.o ./Core/TinyUSB/portable/bridgetek/ft9xx/dcd_ft9xx.su

.PHONY: clean-Core-2f-TinyUSB-2f-portable-2f-bridgetek-2f-ft9xx

