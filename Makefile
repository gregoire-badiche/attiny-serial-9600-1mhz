# Simple Makefile for ATtiny85 examples
# Usage:
#   make basic_io
#   make tx_only
#   make upload-basic_io
#   make upload-tx_only

MCU        = attiny85
F_CPU      = 1000000UL
PROGRAMMER = avrisp
PORT       = /dev/ttyACM0
BAUD	   = 19200

CC      = avr-gcc
OBJCOPY = avr-objcopy
SIZE    = avr-size
CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -std=gnu11 -Isrc

# Examples
EXAMPLES = basic_io tx_only

# Default target: build all examples
all: $(EXAMPLES)

# Build each example (e.g., examples/basic_io/main.c)
$(EXAMPLES):
	@echo "Building $@..."
	$(CC) $(CFLAGS) -o $@.elf src/serial.c examples/$@/main.c
	$(OBJCOPY) -O ihex -R .eeprom $@.elf $@.hex
	$(SIZE) --format=avr --mcu=$(MCU) $@.elf
	@echo "Built: $@.hex"
	@echo

# Upload targets
upload-%: %
	@echo "Uploading $< to $(MCU)..."
	avrdude -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$<.hex:i

# Clean
clean:
	rm -f *.elf *.hex
