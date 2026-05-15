
# avr-gcc -g -mmcu=atmega328p main.c -o main.elf
# avr-objcopy -j .text -j .data -O binary main.elf main.bin
# rm main.elf
# avr-objcopy -j .text -j .data -I binary -O ihex main.bin main.hex 

NAME=main
MCU=atmega328p

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-g -O2 -DF_CPU=16000000UL -mmcu=${MCU}
PORT=/dev/ttyUSB0

SRCS=main.c

all:
	make hex
	make flash

${NAME}.hex:${NAME}.bin
	avr-objcopy -O ihex $< $@

${NAME}.bin: ${SRCS}
	${CC} $(CFLAGS) -mmcu=${MCU} $^ -o $@

hex:$(NAME).hex

flash:$(NAME).hex
	avrdude -v -c arduino -p ${MCU} -P ${PORT} -b 115200 -U flash:w:$<

clean: 
	rm -f ${NAME}.bin ${NAME}.hex
