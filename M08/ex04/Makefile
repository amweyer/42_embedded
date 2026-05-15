
# avr-gcc -g -mmcu=atmega328p main.c -o main.elf
# avr-objcopy -j .text -j .data -O binary main.elf main.bin
# rm main.elf
# avr-objcopy -j .text -j .data -I binary -O ihex main.bin main.hex 


NAME=main
MCU=atmega328p
AVR_LIBC_DIR = \avr\usr\include

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-g -O2 -I$(AVR_LIBC_DIR)/include -DF_CPU=16000000UL -DUART_BAUDRATE=115200 -mmcu=${MCU}
PORT=/dev/ttyUSB*


SRCS=main.c\
		uart.c

OBJS=${SRCS:.c=.o}

all: hex flash

${NAME}.hex:${NAME}.bin
	avr-objcopy -O ihex $< $@
# 	rm -f main.elf

# ${NAME}.bin : ${NAME}.elf   
# 	${OBJCOPY} -O binary $< $@

${NAME}.bin: ${SRCS}
	${CC} $(CFLAGS) -mmcu=${MCU} $^ -o $@

hex:$(NAME).hex

flash:$(NAME).hex
	avrdude -v -c arduino -p ${MCU} -P ${PORT} -b 115200 -U flash:w:$<

screen: all
	screen ${PORT} 115200

clean: 
	rm -f ${NAME}.bin ${NAME}.hex ${OBJS}
