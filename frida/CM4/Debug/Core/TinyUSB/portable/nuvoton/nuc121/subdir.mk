################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/portable/nuvoton/nuc121/dcd_nuc121.c 

OBJS += \
./Core/TinyUSB/portable/nuvoton/nuc121/dcd_nuc121.o 

C_DEPS += \
./Core/TinyUSB/portable/nuvoton/nuc121/dcd_nuc121.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/portable/nuvoton/nuc121/%.o Core/TinyUSB/portable/nuvoton/nuc121/%.su Core/TinyUSB/portable/nuvoton/nuc121/%.cyclo: ../Core/TinyUSB/portable/nuvoton/nuc121/%.c Core/TinyUSB/portable/nuvoton/nuc121/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-portable-2f-nuvoton-2f-nuc121

clean-Core-2f-TinyUSB-2f-portable-2f-nuvoton-2f-nuc121:
	-$(RM) ./Core/TinyUSB/portable/nuvoton/nuc121/dcd_nuc121.cyclo ./Core/TinyUSB/portable/nuvoton/nuc121/dcd_nuc121.d ./Core/TinyUSB/portable/nuvoton/nuc121/dcd_nuc121.o ./Core/TinyUSB/portable/nuvoton/nuc121/dcd_nuc121.su

.PHONY: clean-Core-2f-TinyUSB-2f-portable-2f-nuvoton-2f-nuc121

