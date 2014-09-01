################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/third-party/arduino-serial-lib.c 

OBJS += \
./src/third-party/arduino-serial-lib.o 

C_DEPS += \
./src/third-party/arduino-serial-lib.d 


# Each subdirectory must supply rules for building sources it contributes
src/third-party/%.o: ../src/third-party/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


