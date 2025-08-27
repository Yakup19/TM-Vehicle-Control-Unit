################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/My_Drivers/Canbus_App/Canbus.c 

OBJS += \
./Drivers/My_Drivers/Canbus_App/Canbus.o 

C_DEPS += \
./Drivers/My_Drivers/Canbus_App/Canbus.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/My_Drivers/Canbus_App/%.o Drivers/My_Drivers/Canbus_App/%.su Drivers/My_Drivers/Canbus_App/%.cyclo: ../Drivers/My_Drivers/Canbus_App/%.c Drivers/My_Drivers/Canbus_App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/yakup/OneDrive/Belgeler/GitHub/TM-Vehicle-Control-Unit/FW_TM_VCU2023_Voptimization/Drivers/My_Drivers/Nextion_App" -I"C:/Users/yakup/OneDrive/Belgeler/GitHub/TM-Vehicle-Control-Unit/FW_TM_VCU2023_Voptimization/Drivers/My_Drivers/Canbus_App" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-My_Drivers-2f-Canbus_App

clean-Drivers-2f-My_Drivers-2f-Canbus_App:
	-$(RM) ./Drivers/My_Drivers/Canbus_App/Canbus.cyclo ./Drivers/My_Drivers/Canbus_App/Canbus.d ./Drivers/My_Drivers/Canbus_App/Canbus.o ./Drivers/My_Drivers/Canbus_App/Canbus.su

.PHONY: clean-Drivers-2f-My_Drivers-2f-Canbus_App

