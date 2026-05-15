
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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

    /*Enable transmitter - enables interrupt */
    UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);

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

char switch_case(char c)
{

    if(c >= 'a' && c <= 'z')
        c -= 32;
    else if(c >= 'A' && c <= 'Z')
        c += 32;
    return c;
}


int main(void)
{
    UNSET_BIT(SREG,SREG_I);
    uart_init(UBRRnL);
    SET_BIT(SREG,SREG_I); //    sei();


    while(1)
    {
    }
}

// ISR(USART_RX_vect)

__attribute__((signal))
void USART_RX_vect()
{
    uart_tx(switch_case(UDR0));
}