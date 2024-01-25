################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/TinyUSB/portable/sunxi/dcd_sunxi_musb.c 

OBJS += \
./Core/TinyUSB/portable/sunxi/dcd_sunxi_musb.o 

C_DEPS += \
./Core/TinyUSB/portable/sunxi/dcd_sunxi_musb.d 


# Each subdirectory must supply rules for building sources it contributes
Core/TinyUSB/portable/sunxi/%.o Core/TinyUSB/portable/sunxi/%.su Core/TinyUSB/portable/sunxi/%.cyclo: ../Core/TinyUSB/portable/sunxi/%.c Core/TinyUSB/portable/sunxi/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H747xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../Core/TinyUSB -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-TinyUSB-2f-portable-2f-sunxi

clean-Core-2f-TinyUSB-2f-portable-2f-sunxi:
	-$(RM) ./Core/TinyUSB/portable/sunxi/dcd_sunxi_musb.cyclo ./Core/TinyUSB/portable/sunxi/dcd_sunxi_musb.d ./Core/TinyUSB/portable/sunxi/dcd_sunxi_musb.o ./Core/TinyUSB/portable/sunxi/dcd_sunxi_musb.su

.PHONY: clean-Core-2f-TinyUSB-2f-portable-2f-sunxi

