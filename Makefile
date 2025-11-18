CC = arm-none-eabi-gcc
CFLAGS = -ggdb -DCRAP_HANDLERS_USE_UART -Wall -fjump-tables -mcpu=cortex-m4 -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
LDFLAGS = -Tstm32f411ceu6.ld -nolibc --specs=nosys.specs -nostartfiles
INCLUDES = -Iinc/ -ISDL2 -Iinc/driver/

SRC_DIR = src
BUILD_DIR = build
SOURCES = \
	$(SRC_DIR)/startup_stm32f411ceu6.c\
	$(SRC_DIR)/main.c
BIN = $(BUILD_DIR)/craphal.elf

all: $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(SOURCES) -o $(BIN)

clean:
	rm $(BUILD_DIR)/*

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
