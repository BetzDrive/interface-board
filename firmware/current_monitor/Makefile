LIBS_PATH=STM32CubeL0/Drivers
DEVICE_PATH=$(LIBS_PATH)/CMSIS/Device/ST/STM32L0xx/Include/

SRCS = src/main.c
SRCS += src/system_stm32l0xx.c
SRCS += src/startup_stm32l071xx.s
SRCS += $(LIBS_PATH)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal.c
SRCS += $(LIBS_PATH)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_cortex.c
SRCS += $(LIBS_PATH)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_crc.c
SRCS += $(LIBS_PATH)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_flash.c
SRCS += $(LIBS_PATH)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_gpio.c
SRCS += $(LIBS_PATH)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_rcc.c
SRCS += $(LIBS_PATH)/STM32L0xx_HAL_Driver/Src/stm32l0xx_hal_tim.c

PROJECT ?= build/current_monitor

# specify compiler flags
CFLAGS  = -ggdb -O2 -Wall --specs=nosys.specs
CFLAGS += -TSTM32L071KZTx_FLASH.ld
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -mthumb-interwork
#CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -DSTM32L07X_MD -DUSE_STDPERIPH_DRIVER
CFLAGS += -Wl,--gc-sections
CFLAGS += -I./inc/
CFLAGS += -I$(LIBS_PATH)/STM32L0xx_HAL_Driver/Inc/
CFLAGS += -I$(LIBS_PATH)/CMSIS/Include/
CFLAGS += -I$(LIBS_PATH)/BSP/Components/Common/
CFLAGS += -I$(DEVICE_PATH)

CC = arm-none-eabi-gcc
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
		flash write_image erase $^ 0x8000000;\
		verify_image $^ 0x8000000;\
		reset run;\
		shutdown"

debug: $(PROJECT).elf
	./run_gdb.sh
