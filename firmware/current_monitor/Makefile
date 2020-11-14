# References: https://github.com/starnight/STM32F4/blob/master/FreeRTOS-STM32F4/Makefile

STM_DIR=STM32CubeL0/Drivers

STM_SRC = $(STM_DIR)/STM32L0xx_HAL_Driver/Src

# Tell make to look in that folder if it cannot find a source
# in the current directory
vpath %.c $(STM_SRC)

# Source file
SRCS = src/main.c

# Contains initialisation code and must be compiled into
# our project. This file is in the current directory and
# was writen by ST.
SRCS += src/system_stm32l0xx.c

# Found using STM_SRC
SRCS += stm32l0xx_hal.c
SRCS += stm32l0xx_hal_cortex.c
SRCS += stm32l0xx_hal_crc.c
SRCS += stm32l0xx_hal_flash.c
SRCS += stm32l0xx_hal_gpio.c
SRCS += stm32l0xx_hal_rcc.c
SRCS += stm32l0xx_hal_tim.c

# Startup file written by ST
# The assembly code in this file is the first one to be
# executed. Normally you do not change this file.
SRCS += $(STM_DIR)/CMSIS/Device/ST/STM32L0xx/Source/Templates/gcc/startup_stm32l071xx.s

INC_DIRS = include
INC_DIRS += $(STM_DIR)/STM32L0xx_HAL_Driver/Inc/
INC_DIRS += $(STM_DIR)/CMSIS/Include/
INC_DIRS += $(STM_DIR)/CMSIS/Device/ST/STM32L0xx/Include/
INC_DIRS += $(STM_DIR)/BSP/Components/Common/
INC_DIRS += $(DEVICE_PATH)

PROJECT ?= build/current_monitor

DEFS += -DSTM32L07X_MD -DUSE_STDPERIPH_DRIVER

# directories to be searched for header files
INCLUDE = $(addprefix -I,$(INC_DIRS))

# specify compiler flags
CFLAGS  = -ggdb -O0 -Wall --specs=nosys.specs
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -mthumb-interwork
CFLAGS += -Wl,--gc-sections

LFGLAGS = -TSTM32L071KZTx_FLASH.ld

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

all: $(PROJECT).elf

clean:
	rm build/*

$(PROJECT).elf: $(SRCS)
	mkdir -p build
	$(CC) $(INCLUDE) $(DEFS) $(CFLAGS) $(LFLAGS) $^ -o $@
	$(OBJCOPY) -O ihex $(PROJECT).elf $(PROJECT).hex
	$(OBJCOPY) -O binary $(PROJECT).elf $(PROJECT).bin

upload: $(PROJECT).bin
	openocd -f openocd.cfg -c "\
		init;\
		reset halt;\
		flash probe 0;\
		flash write_image erase $^ 0x8000000;\
		verify_image $^ 0x8000000;\
		reset run;\
		shutdown"

# Flash the STM32F4
flash: 
	st-flash --debug write $(PROJ_NAME).bin 0x8000000

debug: $(PROJECT).elf
	./run_gdb.sh
