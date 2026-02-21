CC = arm-none-eabi-gcc
CFLAGS = -ggdb -Wall -Wextra -fjump-tables -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
LDFLAGS = -Tstm32f411ceu6.ld -nolibc --specs=nosys.specs -nostartfiles  
INCLUDES = -Iinc/

SRC_DIR = src
BUILD_DIR = build

SOURCES = $(SRC_DIR)/startup_stm32f411ceu6.c 

MAIN_SRC = $(SOURCES) $(SRC_DIR)/main.c
EXTI_SRC = $(SOURCES) tests/exti.c
TIMER_SRC = $(SOURCES) tests/timer.c
RAMFUNC_SRC = $(SOURCES) tests/ramfunc.c
IVTRELOC_SRC = $(SOURCES) tests/ivt_reloc.c
UART_SRC = $(SOURCES) tests/uart.c

MAIN_BIN = $(BUILD_DIR)/main.elf
EXTI_BIN = $(BUILD_DIR)/exti.elf
TIMER_BIN = $(BUILD_DIR)/timer.elf
RAMFUNC_BIN = $(BUILD_DIR)/ramfunc.elf
IVTRELOC_BIN = $(BUILD_DIR)/ivtreloc.elf
UART_BIN = $(BUILD_DIR)/uart.elf


PRIMARY_GOAL := $(firstword $(MAKECMDGOALS))

PRIMARY_GOAL_UPPER := $(shell echo $(PRIMARY_GOAL) | tr a-z A-Z)

CURRBIN := $($(PRIMARY_GOAL_UPPER)_BIN)

###############
# Build rules #
###############

.DEFAULT_GOAL := help

.PHONY: help
help:
	$(error No target specified. Example: make exti or make exti debug)


$(MAIN_BIN): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(MAIN_SRC) -o $@

$(EXTI_BIN): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(EXTI_SRC) -o $@

$(TIMER_BIN): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(TIMER_SRC) -o $@

$(RAMFUNC_BIN): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(RAMFUNC_SRC) -o $@

$(IVTRELOC_BIN): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(IVTRELOC_SRC) -o $@

$(UART_BIN): $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(UART_SRC) -o $@

.PHONY: main
main: $(MAIN_BIN)

.PHONY: exti
exti: $(EXTI_BIN)

.PHONY: timer
timer: $(TIMER_BIN)

.PHONY: ramfunc
ramfunc:$(RAMFUNC_BIN)

.PHONY: ivtreloc
ivtreloc:$(IVTRELOC_BIN)

.PHONY: uart
uart: $(UART_BIN)

.PHONY: debug
debug:
ifeq ($(CURRBIN),)
	$(error "debug must follow a build target, e.g. `make exti debug`")
endif

	@echo "Flashing $(CURRBIN)..."
	openocd -f /usr/share/openocd/scripts/interface/stlink.cfg \
	-f /usr/share/openocd/scripts/target/stm32f4x.cfg \
		-c "program $(CURRBIN) reset exit"

	@echo "Starting OpenOCD server..."
	openocd -f /usr/share/openocd/scripts/interface/stlink.cfg \
	-f /usr/share/openocd/scripts/target/stm32f4x.cfg & \
	gf2 $(CURRBIN) \
		-ex "target remote localhost:3333" \
		-ex "monitor reset halt" 
	
	pkill openocd



###############
# Clean       #
###############
.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.elf

###############
# Build dir   #
###############
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
