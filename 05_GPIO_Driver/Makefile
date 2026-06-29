# Project Name
TARGET = firmware

# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Directories
SRC_DIR = src
INC_DIR = include
STARTUP_DIR = startup
LINKER_DIR = linker
BUILD_DIR = build

# CPU Options
CPU = -mcpu=cortex-m4
THUMB = -mthumb

MCU = $(CPU) $(THUMB)

# Compiler Flags
CFLAGS = \
$(MCU) \
-std=c11 \
-Wall \
-O0 \
-g \
-ffreestanding \
-ffunction-sections \
-fdata-sections \
-I$(INC_DIR)

# Source Files
C_SOURCES := $(wildcard $(SRC_DIR)/*.c)

# Object Files
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
OBJECTS += $(BUILD_DIR)/startup_stm32f407.o

# Linker Flags
LDFLAGS = \
$(MCU) \
-T$(LINKER_DIR)/stm32f407.ld \
-nostartfiles \
-Wl,-Map=$(BUILD_DIR)/firmware.map,--gc-sections

all: $(BUILD_DIR) $(TARGET).elf $(TARGET).bin $(TARGET).hex

# Build directory
$(BUILD_DIR):
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

# Compile C source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble startup file
$(BUILD_DIR)/startup_stm32f407.o: $(STARTUP_DIR)/startup_stm32f407.s
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# Generate bin
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Generate hex
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# Size
size: $(TARGET).elf
	$(SIZE) $<

# Clean
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET).elf $(TARGET).bin $(TARGET).hex

.PHONY: all clean size
