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

DEFS += -DSTM32L071xx -DUSE_STDPERIPH_DRIVER

# directories to be searched for header files
INCLUDE = $(addprefix -I,$(INC_DIRS))

# Debug flags
DFLAGS = -g2 -ggdb

# specify compiler flags
CFLAGS  = -std=gnu99 -N -O0 -Wall --specs=nosys.specs
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0plus -mthumb-interwork
CFLAGS += -Wl,--gc-sections

LFLAGS = -Wl,-Map=$(PROJECT).map,--cref -T flash.ld

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

all: $(PROJECT).elf

clean:
	rm build/*

$(PROJECT).elf: $(SRCS)
	mkdir -p build
	$(CC) $(INCLUDE) $(DEFS) $(DFLAGS) $(CFLAGS) $(LFLAGS) $^ -o $@
	$(OBJCOPY) -O ihex $(PROJECT).elf $(PROJECT).hex
	$(OBJCOPY) -O binary $(PROJECT).elf $(PROJECT).bin
	$(OBJDUMP) -S -d $(PROJECT).elf > $(PROJECT).objdump

upload: $(PROJECT).bin
	openocd -f openocd.cfg -c "\
		init;\
		reset halt;\
		flash probe 0;\
		flash write_image erase $^ 0x08000000;\
		verify_image $^ 0x08000000;\
		reset run;\
		shutdown"

flash_olimex: $(PROJECT).bin
	openocd -f interface/ftdi/olimex-arm-usb-tiny-h.cfg -f interface/ftdi/olimex-arm-jtag-swd.cfg -f target/stm32l0.cfg -c "\
		reset_config srst_only srst_nogate connect_assert_srst; \
		init; \
		sleep 100; \
		reset halt; \
		sleep 100; \
		flash probe 0; \
		flash write_image erase $^ 0x08000000;\
		verify_image $^ 0x08000000;\
		sleep 100; \
		reset run; \
		sleep 100; \
		shutdown"

# Flash the STM32F4
flash: 
	st-flash --debug write $(PROJ_NAME).bin 0x08000000

debug: $(PROJECT).elf
	gdb-multiarch --eval-command="target extended-remote localhost:3333" \
		build/current_monitor.elf
	#./run_gdb.sh
