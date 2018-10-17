################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/src/api/api_lib.c \
../Middlewares/Third_Party/LwIP/src/api/api_msg.c \
../Middlewares/Third_Party/LwIP/src/api/err.c \
../Middlewares/Third_Party/LwIP/src/api/netbuf.c \
../Middlewares/Third_Party/LwIP/src/api/netdb.c \
../Middlewares/Third_Party/LwIP/src/api/netifapi.c \
../Middlewares/Third_Party/LwIP/src/api/sockets.c \
../Middlewares/Third_Party/LwIP/src/api/tcpip.c 

OBJS += \
./Middlewares/Third_Party/LwIP/src/api/api_lib.o \
./Middlewares/Third_Party/LwIP/src/api/api_msg.o \
./Middlewares/Third_Party/LwIP/src/api/err.o \
./Middlewares/Third_Party/LwIP/src/api/netbuf.o \
./Middlewares/Third_Party/LwIP/src/api/netdb.o \
./Middlewares/Third_Party/LwIP/src/api/netifapi.o \
./Middlewares/Third_Party/LwIP/src/api/sockets.o \
./Middlewares/Third_Party/LwIP/src/api/tcpip.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/src/api/api_lib.d \
./Middlewares/Third_Party/LwIP/src/api/api_msg.d \
./Middlewares/Third_Party/LwIP/src/api/err.d \
./Middlewares/Third_Party/LwIP/src/api/netbuf.d \
./Middlewares/Third_Party/LwIP/src/api/netdb.d \
./Middlewares/Third_Party/LwIP/src/api/netifapi.d \
./Middlewares/Third_Party/LwIP/src/api/sockets.d \
./Middlewares/Third_Party/LwIP/src/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/src/api/%.o: ../Middlewares/Third_Party/LwIP/src/api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F767xx -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Inc" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Drivers/STM32F7xx_HAL_Driver/Inc" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Drivers/CMSIS/Device/ST/STM32F7xx/Include" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/FreeRTOS/Source/include" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Drivers/CMSIS/Include" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Common/include" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/LwIP/src/include" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/LwIP/system" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/LwIP/src/include/netif/ppp" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/LwIP/src/include/lwip" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/LwIP/src/include/lwip/apps" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/LwIP/src/include/lwip/priv" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/LwIP/src/include/lwip/prot" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/LwIP/src/include/netif" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/LwIP/src/include/posix" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/LwIP/src/include/posix/sys" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Middlewares/Third_Party/LwIP/system/arch" -I"D:/Users/Tyler/Documents/STM/embedded/soccer-embedded/Robot_F7/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


