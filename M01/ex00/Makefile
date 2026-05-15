
NAME=main
MCU=atmega328p

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-g -DF_CPU=16000000UL -mmcu=${MCU}
PORT=/dev/ttyUSB0

SRCS=main.c
OBJS=${SRCS:.c=.o}

all:
	make hex
	make flash

${NAME}.hex:${NAME}.bin
	avr-objcopy -O ihex $< $@

${NAME}.bin: ${SRCS}
	${CC} $(CFLAGS) $^ -o $@

hex:$(NAME).hex

flash:$(NAME).hex
	avrdude -v -c arduino -p ${MCU} -P ${PORT} -b 115200 -U flash:w:$<

clean: 
	rm -f ${NAME}.bin ${NAME}.hex ${OBJS}
