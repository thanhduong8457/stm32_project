DEVICE = STM32F407xG
FLASH  = 0x08000000

USE_ST_CMSIS = true

# Check to make sure that the required variables are set
ifndef DEVICE
    $(error Please set the required DEVICE variable in your makefile.)
endif

ifndef FLASH
    $(error Please set the required FLASH variable in your makefile.)
endif

# Standard values for (linked) STM32-base folders
STM32_BASE_PATH   ?= .
#STM32_CUBE_PATH   ?= ./STM32-base-STM32Cube

# STM32-base sub-folders
BASE_LINKER   = $(STM32_BASE_PATH)/linker
BASE_STARTUP  = $(STM32_BASE_PATH)/startup

# Standard values for project folders
BIN_FOLDER ?= ./bin
OBJ_FOLDER ?= ./obj
SRC_FOLDER ?= ./src
INC_FOLDER ?= ./src

SERIES_CPU  = cortex-m4
SERIES_ARCH = armv7e-m+fp

MAPPED_DEVICE = STM32F407xx

MAPPED_DEVICE ?= $(DEVICE)

# The toolchain path, defaults to using the globally installed toolchain
ifdef TOOLCHAIN_PATH
    TOOLCHAIN_SEPARATOR = /
endif

TOOLCHAIN_PATH      ?= #/mnt/c/VSARM/gcc-arm-none-eabi-10.3-2021.10/bin/
TOOLCHAIN_SEPARATOR ?=
TOOLCHAIN_PREFIX    ?= arm-none-eabi-

CC      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)gcc
CXX     = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)g++
LD      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)ld -v
AR      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)ar
AS      = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)gcc
OBJCOPY = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)objcopy
OBJDUMP = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)objdump
SIZE    = $(TOOLCHAIN_PATH)$(TOOLCHAIN_SEPARATOR)$(TOOLCHAIN_PREFIX)size


# Flags - Overall Options
CPPFLAGS += -std=gnu++14
CPPFLAGS += -specs=nosys.specs

# Flags - C Language Options
CFLAGS += -ffreestanding

# Flags - C++ Language Options
CXXFLAGS += -fno-threadsafe-statics
CXXFLAGS += -fno-rtti
CXXFLAGS += -fno-exceptions
CXXFLAGS += -fno-unwind-tables

# Flags - Warning Options
CPPFLAGS += -Wall
CPPFLAGS += -Wextra

# Flags - Debugging Options
CPPFLAGS += -g

# Flags - Optimization Options
CPPFLAGS += -ffunction-sections
CPPFLAGS += -fdata-sections

# Flags - Preprocessor options
CPPFLAGS += -D $(MAPPED_DEVICE)

# Flags - Assembler Options
ifneq (,$(or USE_ST_CMSIS, USE_ST_HAL))
    CPPFLAGS += -Wa,--defsym,CALL_ARM_SYSTEM_INIT=1
endif

# Flags - Linker Options
# CPPFLAGS += -nostdlib
CPPFLAGS += -Wl,-L$(BASE_LINKER),-T$(BASE_LINKER)/$(DEVICE).ld

# Flags - Directory Options
CPPFLAGS += -I$(INC_FOLDER)
CPPFLAGS += -I$(BASE_STARTUP)
#include the ROS librabry file
#CPPFLAGS += -I./Inc/actionlib

# Flags - Machine-dependant options
CPPFLAGS += -mcpu=$(SERIES_CPU)
CPPFLAGS += -march=$(SERIES_ARCH)
CPPFLAGS += -mlittle-endian
CPPFLAGS += -mthumb
CPPFLAGS += -masm-syntax-unified

# Output files
ELF_FILE_NAME ?= stm32_executable.elf
BIN_FILE_NAME ?= stm32_bin_image.bin
HEX_FILE_NAME ?= stm32_hex_image.hex
OBJ_FILE_NAME ?= startup_$(MAPPED_DEVICE).o

ELF_FILE_PATH = $(BIN_FOLDER)/$(ELF_FILE_NAME)
BIN_FILE_PATH = $(BIN_FOLDER)/$(BIN_FILE_NAME)
HEX_FILE_PATH = $(BIN_FOLDER)/$(HEX_FILE_NAME)
OBJ_FILE_PATH = $(OBJ_FOLDER)/$(OBJ_FILE_NAME)

# Input files
SRC ?=
SRC += $(SRC_FOLDER)/*.c
SRC += $(SRC_FOLDER)/*.cpp

# Startup file
DEVICE_STARTUP = $(BASE_STARTUP)/$(MAPPED_DEVICE).s

# Include the CMSIS files
CPPFLAGS += -I./STM32F4_Driver/CMSIS/Include
CPPFLAGS += -I./STM32F4_Driver/CMSIS/Device/ST/STM32F4xx/Include

# Include the HAL files
CPPFLAGS += -D USE_STDPERIPH_DRIVER
CPPFLAGS += -D STM32F40_41xxx
CPPFLAGS += -D HSE_VALUE=8000000
CPPFLAGS += -I./STM32F4_Driver/STM32F4xx_StdPeriph_Driver/inc

# A simply expanded variable is used here to perform the find command only once.
STD_SRC := $(shell find ./STM32F4_Driver/STM32F4xx_StdPeriph_Driver/src/*.c ! -name '*_template.c')
SRC += $(STD_SRC)

# SRC += \
# ./STM32F4_Driver/STM32F4xx_StdPeriph_Driver/src/misc.c \
# ./STM32F4_Driver/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.c \
# ./STM32F4_Driver/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c \
# ./STM32F4_Driver/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c \
# ./STM32F4_Driver/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c \
# ./STM32F4_Driver/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c \
# ./STM32F4_Driver/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c

# Make all
all:$(BIN_FILE_PATH)

$(BIN_FILE_PATH): $(ELF_FILE_PATH)
	$(SIZE) $(ELF_FILE_PATH)
	$(OBJCOPY) -O binary $^ $@
	$(OBJCOPY) -O ihex $(ELF_FILE_PATH) $(HEX_FILE_PATH)

$(ELF_FILE_PATH): $(SRC) $(OBJ_FILE_PATH) | $(BIN_FOLDER)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(OBJ_FILE_PATH): $(DEVICE_STARTUP) | $(OBJ_FOLDER)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(BIN_FOLDER):
	mkdir $(BIN_FOLDER)

$(OBJ_FOLDER):
	mkdir $(OBJ_FOLDER)

# Make clean
clean:
	rm -f $(ELF_FILE_PATH)
	rm -f $(BIN_FILE_PATH)
	rm -f $(HEX_FILE_PATH)
	rm -f $(OBJ_FILE_PATH)

# Make flash
flash:
	st-flash write $(BIN_FOLDER)/$(BIN_FILE_NAME) $(FLASH)

.PHONY: all clean flash
