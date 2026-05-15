
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "mapping.h"
#include "helper.h"
#include "uart.h"

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
    UCSR0B = (1<<TXEN0) | (1<<RXEN0);

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

char uart_rx( void )
{
    /* Wait for data to be received */
    while ( !(UCSR0A & (1<<RXC0)) )
    ;
    /* Get and return received data from buffer */
    return UDR0;
}

void uart_printstr(const char* str)
{

    while (*str > 0)
        uart_tx(*str++);
}


//addition

int valid_char(unsigned char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '#')
        return (1);
    return (0);
}

void uart_getstr(char field[13])
{
    int eol = 0;
    int i = 0;
    char tmp;

    while (!eol)
    {
        tmp = uart_rx();

        if (tmp == '\r')//ENTER
            eol = 1;
        else if (tmp == 127 && i)//BACKSPAPCE
        {
                uart_printstr("\b \b");
                field[--i] = 0;
        }
        else if (i < 12 && valid_char(tmp))//VALIDCHAR TO ADD
        {
            //WRITE
            field[i++] = tmp;
            //ECHO
            uart_tx(tmp);
        }
    }
    field[i] = 0;//null terminate
    uart_printstr("\r\n");//newline
}

int str_comp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int valid_color_str()
{
    if (!str_comp(color_str, "#FULLRAINBOW"))
        return (1);
	if (color_str[0] != '#')
		return (0);
	for (uint8_t i = 1; i < 7; i++)
	{
        if (!((color_str[i] >= 'A' && color_str[i] <= 'F') || (color_str[i] >= '0' && color_str[i] <= '9')))
			return (0);
	}
    if (color_str[7] != 'D')
		return (0);
    if (color_str[8] != '6' && color_str[8] != '7' && color_str[8] != '8')
		return (0);
    if (color_str[9])
        return (0);
	return (1);
}

uint8_t atoi_hex(char *clr)
{
	uint8_t res = 0;
	for (uint8_t i = 0; i < 2; i++)
	{
		if (clr[i] >= 'A' && clr[i] <= 'F')
			res = res * 16 + (clr[i] - 'A' + 10);
		else
			res = res * 16 + (clr[i] - '0'); 
	}
	return res;	
}



void to_upper(char *str)
{
	while (*str++)
	{
		if (*str >= 'a' && *str <= 'z')
			*str -= 32;
	}
}

char color_str[13] = {0};

void get_input()
{
	int valid = 0;
	while (!valid)
	{
        uart_printstr("type command\r\n");
		uart_getstr(color_str);
        to_upper(color_str);
		if (valid_color_str())
			valid++;
		else
			uart_printstr("ERROR\r\n");
	}
}