
#include <avr/io.h>
#include <util/delay.h>

#include "mapping.h"
#include "helper.h"

// #define F_CPU 16000000
// #define UART_BAUDRATE 115200
// #define UBRRnL (F_CPU/16/UART_BAUDRATE-1)
// #define UBRRnL ((F_CPU) / (16 * UART_BAUDRATE)) - 1

#define UBRRnL 8 //p199


/* 16.11 Register Description */

/* on veut 115200 8N1 => Baud rate:115200 - 8bits de donnees - pas de parite(N) - 1bit de stop

screen /dev/ttyUSB0 115200

*/


void uart_init(unsigned int ubrr)
{

    /*Set baud rate */
    UBRR0H = (unsigned char)(ubrr >> 8); //write higher
    UBRR0L = (unsigned char)ubrr; //write lower bit

    /*Enable transmitter */
    UCSR0B = (1<<TXEN0);

    /* Set frame format: 8data, 1stop bit */
    UCSR0C = (3 <<UCSZ00);
}


void uart_tx(char c)
{
    // wait for transmit buffer to be empty
    while(!(UCSR0A & (1 << UDRE0)));

    // load c into transmit register
    UDR0 = c;
}

int main(void)
{
    uart_init(UBRRnL);

    while(1)
    {
        uart_tx('z');
        _delay_ms(1000);
    }

}

