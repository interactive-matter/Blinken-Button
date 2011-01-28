# Name: Makefile
# Author: Marcus Nowotny
#  http://interactive-matter.org/
#
#  This file is part of Blinken Button.
#
#  Blinken Button is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Blinken Button for Beginners is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  You should have received a copy of the GNU General Public License
#  along with Blinken Buttons for Beginners.  If not, see <http://www.gnu.org/licenses/>.
#

# This are the programming options. Be sure to adapt it to your programmer and
# the port the programmer is connected to.
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# PROGRAMMER_PORT ...  the port the programmer is attached to. This is highly 
#                dependent on your programmer and operating system.
#                This settings works for my mac

PROGRAMMER = avrispmkII
PROGRAMMER_PORT = usb

# This are the hardware settings for the Blinken Buttons for Beginners. There is
# normally no need tho change these (or anything below) (if you do not change 
# the hardware of the Blinken Buttons for Beginners, what is quite improbable).
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     = atmega168
CLOCK      = 8000000
OBJECTS    = main.o rendering.o display.o random.o state.o core-flash-content.o custom-flash-content.o 
FUSES      = -Ulfuse:w:0xe2:m -Uhfuse:w:0xdf:m -Uefuse:w:0x1:m


# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE) -P $(PROGRAMMER_PORT)
COMPILE = avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
