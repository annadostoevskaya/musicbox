CC=avr-gcc
AVRDUDE_FLAGS?=
EMU=emulator.exe

all: main.hex

main.hex: main.elf
	avr-size main.elf
	avr-objcopy -O ihex -R .eeprom main.elf main.hex

main.elf: main.c
	avr-gcc -Wall -Wextra -Os -Og -mmcu=attiny13a -o main.elf main.c

clean:
	del main.hex main.elf main.o emu.log

# avrdude: main.hex
# 	avrdude -p attiny13a -P usb -c usbasp -e flash -i 

emu: main.hex
	emulator.exe -lifetime 1024 -logall -logfile emu.log main.hex

