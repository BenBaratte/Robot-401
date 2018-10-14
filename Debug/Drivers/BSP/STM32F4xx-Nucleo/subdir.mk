################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.c 

OBJS += \
./Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.o 

C_DEPS += \
./Drivers/BSP/STM32F4xx-Nucleo/stm32f4xx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32F4xx-Nucleo/%.o: ../Drivers/BSP/STM32F4xx-Nucleo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F401xE -I"C:/Work/Ressources/Project Robot/Project Robot F401 test/Inc" -I"C:/Work/Ressources/STM32CubeExpansion_SPN4_V1.5.0/Drivers/BSP/Components/Common" -I"C:/Work/Ressources/Project Robot/Project Robot F401 test/Drivers/BSP/l6206" -I"C:/Work/Ressources/Project Robot/Project Robot F401 test/Drivers/BSP/X-NUCLEO-IHMxx" -I"C:/Work/Ressources/Project Robot/Project Robot F401 test/Drivers/BSP/STM32F4xx-Nucleo" -I"C:/Work/Ressources/Project Robot/Project Robot F401 test/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Work/Ressources/Project Robot/Project Robot F401 test/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Work/Ressources/Project Robot/Project Robot F401 test/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Work/Ressources/Project Robot/Project Robot F401 test/Drivers/CMSIS/Include"  -O0 -g -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


