################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Core/TinyUSB/typec/subdir.mk
-include Core/TinyUSB/portable/wch/subdir.mk
-include Core/TinyUSB/portable/valentyusb/eptri/subdir.mk
-include Core/TinyUSB/portable/ti/msp430x5xx/subdir.mk
-include Core/TinyUSB/portable/template/subdir.mk
-include Core/TinyUSB/portable/synopsys/dwc2/subdir.mk
-include Core/TinyUSB/portable/sunxi/subdir.mk
-include Core/TinyUSB/portable/st/typec/subdir.mk
-include Core/TinyUSB/portable/st/stm32_fsdev/subdir.mk
-include Core/TinyUSB/portable/sony/cxd56/subdir.mk
-include Core/TinyUSB/portable/renesas/rusb2/subdir.mk
-include Core/TinyUSB/portable/raspberrypi/rp2040/subdir.mk
-include Core/TinyUSB/portable/raspberrypi/pio_usb/subdir.mk
-include Core/TinyUSB/portable/ohci/subdir.mk
-include Core/TinyUSB/portable/nxp/lpc_ip3511/subdir.mk
-include Core/TinyUSB/portable/nxp/lpc17_40/subdir.mk
-include Core/TinyUSB/portable/nxp/khci/subdir.mk
-include Core/TinyUSB/portable/nuvoton/nuc505/subdir.mk
-include Core/TinyUSB/portable/nuvoton/nuc121/subdir.mk
-include Core/TinyUSB/portable/nuvoton/nuc120/subdir.mk
-include Core/TinyUSB/portable/nordic/nrf5x/subdir.mk
-include Core/TinyUSB/portable/mindmotion/mm32/subdir.mk
-include Core/TinyUSB/portable/microchip/samx7x/subdir.mk
-include Core/TinyUSB/portable/microchip/samg/subdir.mk
-include Core/TinyUSB/portable/microchip/samd/subdir.mk
-include Core/TinyUSB/portable/microchip/pic32mz/subdir.mk
-include Core/TinyUSB/portable/microchip/pic/subdir.mk
-include Core/TinyUSB/portable/mentor/musb/subdir.mk
-include Core/TinyUSB/portable/espressif/esp32sx/subdir.mk
-include Core/TinyUSB/portable/ehci/subdir.mk
-include Core/TinyUSB/portable/dialog/da146xx/subdir.mk
-include Core/TinyUSB/portable/chipidea/ci_hs/subdir.mk
-include Core/TinyUSB/portable/chipidea/ci_fs/subdir.mk
-include Core/TinyUSB/portable/bridgetek/ft9xx/subdir.mk
-include Core/TinyUSB/portable/analog/max3421/subdir.mk
-include Core/TinyUSB/host/subdir.mk
-include Core/TinyUSB/device/subdir.mk
-include Core/TinyUSB/common/subdir.mk
-include Core/TinyUSB/class/video/subdir.mk
-include Core/TinyUSB/class/vendor/subdir.mk
-include Core/TinyUSB/class/usbtmc/subdir.mk
-include Core/TinyUSB/class/net/subdir.mk
-include Core/TinyUSB/class/msc/subdir.mk
-include Core/TinyUSB/class/midi/subdir.mk
-include Core/TinyUSB/class/hid/subdir.mk
-include Core/TinyUSB/class/dfu/subdir.mk
-include Core/TinyUSB/class/cdc/subdir.mk
-include Core/TinyUSB/class/bth/subdir.mk
-include Core/TinyUSB/class/audio/subdir.mk
-include Core/TinyUSB/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Common/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := frida_CM4
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
frida_CM4.elf \

MAP_FILES += \
frida_CM4.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
frida_CM4.list \


# All Target
all: main-build

# Main-build Target
main-build: frida_CM4.elf secondary-outputs

# Tool invocations
frida_CM4.elf frida_CM4.map: $(OBJS) $(USER_OBJS) C:\Users\pique_n\Documents\GitHub\frida\frida\CM4\STM32H747XIHX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "frida_CM4.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\pique_n\Documents\GitHub\frida\frida\CM4\STM32H747XIHX_FLASH.ld" --specs=nosys.specs -Wl,-Map="frida_CM4.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

frida_CM4.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "frida_CM4.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout frida_CM4.elf frida_CM4.list frida_CM4.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
