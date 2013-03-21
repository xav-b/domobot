ARDUINO_DIR = /usr/share/arduino
AVR_TOOLS_PATH = /usr/bin
#TARGET = blink
MCU = atmega328p
F_CPU = 16000000
#TODO try USB0 when ACM0 fails
ARDUINO_PORT = /dev/ttyACM0
AVRDUDE_ARD_PROGRAMMER = arduino
AVRDUDE_ARD_BAUDRATE = 115200
PGMER = -c $(AVRDUDE_ARD_PROGRAMMER) -b $(AVRDUDE_ARD_BAUDRATE) -P $(ARDUINO_PORT)
DUDE = /usr/bin/avrdude -V -p $(MCU)
GDBINITFILE = gdbinit-$(PROJECT)

## Stalker stuff ##
BOARD_TAG = atmega328
ISP_PORT = /dev/ttyACM0
ISP_PROG = -c usbtiny
AVRDUDE_ISP_OPTS = $(ISP_PROG)
#! Default fuses, see Arduino.mk

include ./Arduino.mk

install: all size load

load: $(TARGET_HEX)
	$(DUDE) $(PGMER) -U flash:w:$(TARGET_HEX)

customSize:
	$(SIZE) -C --mcu=$(MCU) build-cli/$(TARGET).elf

#stalkerLoad: $(TARGET_HEX)
	#$(AVRDUDE) $(AVRDUDE_COM_OPTS) $(ISP_PROG) -B 1 -U flash:w:$(TARGET_HEX)

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
