################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/typec/usbc.c 

OBJS += \
./Core/TinyUSB/typec/usbc.o 

C_DEPS += \
./Core/TinyUSB/typec/usbc.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/typec/%.o Core/TinyUSB/typec/%.su Core/TinyUSB/typec/%.cyclo: ../Core/TinyUSB/typec/%.c Core/TinyUSB/typec/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-typec

clean-Core-2f-TinyUSB-2f-typec:
	-$(RM) ./Core/TinyUSB/typec/usbc.cyclo ./Core/TinyUSB/typec/usbc.d ./Core/TinyUSB/typec/usbc.o ./Core/TinyUSB/typec/usbc.su

.PHONY: clean-Core-2f-TinyUSB-2f-typec

