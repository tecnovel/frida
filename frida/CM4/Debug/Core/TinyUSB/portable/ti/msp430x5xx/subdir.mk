################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.c 

OBJS += \
./Core/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.o 

C_DEPS += \
./Core/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/portable/ti/msp430x5xx/%.o Core/TinyUSB/portable/ti/msp430x5xx/%.su Core/TinyUSB/portable/ti/msp430x5xx/%.cyclo: ../Core/TinyUSB/portable/ti/msp430x5xx/%.c Core/TinyUSB/portable/ti/msp430x5xx/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-portable-2f-ti-2f-msp430x5xx

clean-Core-2f-TinyUSB-2f-portable-2f-ti-2f-msp430x5xx:
	-$(RM) ./Core/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.cyclo ./Core/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.d ./Core/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.o ./Core/TinyUSB/portable/ti/msp430x5xx/dcd_msp430x5xx.su

.PHONY: clean-Core-2f-TinyUSB-2f-portable-2f-ti-2f-msp430x5xx

