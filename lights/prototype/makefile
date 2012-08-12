ARDUINO_DIR = /usr/share/arduino
TARGET = blink
MCU = atmega328p
F_CPU = 16000000
ARDUINO_PORT = /dev/ttyACM0
AVRDUDE_ARD_PROGRAMMER = arduino
#for ISP: avrispv2
AVRDUDE_ARD_BAUDRATE = 115200
PGMER = -c $(AVRDUDE_ARD_PROGRAMMER) -b $(AVRDUDE_ARD_BAUDRATE) -P $(ARDUINO_PORT)
DUDE = /usr/bin/avrdude -V -p $(MCU)
GDBINITFILE = gdbinit-$(PROJECT)
SIZE = avr-size
CC = avr-gcc

include ./Arduino.mk

load: $(TARGET_HEX)
	$(DUDE) $(PGMER) -U flash:w:$(TARGET_HEX)

size:
	$(SIZE) -C --mcu=$(MCU) build-cli/$(TARGET).elf

install: all size load

fuse:
	avrdude -p $(MCU) -c $(AVRDUDE_ARD_PROGRAMMER) -P $(ARDUINO_PORT) -v -U lfuse:w:0xc6:m -U hfuse:w:0xd9:m

ddd: gdbinit
	ddd --debugger "avr-gdb -x $(GDBINITFILE)"

gdbserver: gdbinit
	simulavr --device $(MCU) --gdbserver

gdbinit: $(GDBINITFILE)

$(GDBINITFILE): $(TARGET_HEX)
	@echo "file $(TARGET).elf" > $(GDBINITFILE)

	@echo "target remote localhost:1212" >> $(GDBINITFILE)
	@echo "load" >> $(GDBINITFILE)
	@echo "break main" >> $(GDBINITFILE)
	@echo "continue" >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"

gccversion:
	@$(CC) --version
