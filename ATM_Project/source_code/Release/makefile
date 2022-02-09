################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include MCAL/MDIO/subdir.mk
-include Free_RTOS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
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

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
atm_pro.lss \

FLASH_IMAGE += \
atm_pro.hex \

EEPROM_IMAGE += \
atm_pro.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: atm_pro.elf secondary-outputs

# Tool invocations
atm_pro.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,atm_pro.map -mmcu=atmega32 -o "atm_pro.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

atm_pro.lss: atm_pro.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S atm_pro.elf  >"atm_pro.lss"
	@echo 'Finished building: $@'
	@echo ' '

atm_pro.hex: atm_pro.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex atm_pro.elf  "atm_pro.hex"
	@echo 'Finished building: $@'
	@echo ' '

atm_pro.eep: atm_pro.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex atm_pro.elf  "atm_pro.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: atm_pro.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 atm_pro.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) atm_pro.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
