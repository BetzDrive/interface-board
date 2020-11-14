LIBS_PATH=STM32CubeL0/Drivers
DEVICE_PATH=$(LIBS_PATH)/CMSIS/Device/ST/STM32L0xx/Include/

SRCS = main.c
SRCS += $(LIBS_PATH)/STM32L0xx_HAL_Driver/Src/stm32l0xx_ll_gpio.c

PROJECT ?= build/current_monitor

# specify compiler flags
CFLAGS  = -g -O2 -Wall
CFLAGS += -TSTM32L071KZTx_FLASH.ld
#CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0+ -mthumb-interwork
#CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
#CFLAGS += -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
CFLAGS += -Wl,--gc-sections
CFLAGS += -I.
CFLAGS += -I$(LIBS_PATH)/STM32L0xx_HAL_Driver/Inc/
CFLAGS += -I$(LIBS_PATH)/CMSIS/Include/
CFLAGS += -I$(LIBS_PATH)/BSP/Components/Common/
CFLAGS += -I$(DEVICE_PATH)

CC = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy

all: $(PROJECT).elf

clean:
	rm build/*

$(PROJECT).elf: $(SRCS)
	mkdir -p build
	$(CC) $(CFLAGS) $^ -o $@
	$(OBJCOPY) -O ihex $(PROJECT).elf $(PROJECT).hex
	$(OBJCOPY) -O binary $(PROJECT).elf $(PROJECT).bin

upload: $(PROJECT).bin
	openocd -f openocd.cfg -c "\
		init;\
		reset halt;\
		flash probe 0;\
		flash write_image erase $(PROJECT).bin 0x08000000;\
		verify_image $(PROJECT).bin 0x08000000;\
		reset run;\
		shutdown"
