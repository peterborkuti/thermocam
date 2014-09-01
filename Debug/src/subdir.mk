################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/digit.cpp \
../src/main.cpp \
../src/segment.cpp \
../src/util.cpp 

OBJS += \
./src/digit.o \
./src/main.o \
./src/segment.o \
./src/util.o 

CPP_DEPS += \
./src/digit.d \
./src/main.d \
./src/segment.d \
./src/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/home/peter/ocvinst/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


