# name of the application
export PROGRAM_NAME:=avr_servo

# toolchain
export CC :=avr-gcc
export CXX:=avr-g++
export OBJCOPY:=avr-objcopy
export LD :=$(CXX)

# tools settings
export MCU_TYPE:=atmega8
export MCU_PROG:=m8

# extra flags, if needed
export CFLAGS  +=-mmcu=$(MCU_TYPE)
export CXXFLAGS+=-mmcu=$(MCU_TYPE)
export LDFLAGS +=-mmcu=$(MCU_TYPE)
