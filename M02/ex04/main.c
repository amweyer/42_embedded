
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
#define USER "amaya\0"
#define PASS "lula\0"
/* 16.11 Register Description */

/* on veut 115200 8N1 => Baud rate:115200 - 8bits de donnees - pas de parite(N) - 1bit de stop
screen /dev/ttyUSB0 115200
*/

char username[15];
char password[15];
uint8_t buf = 0;
char c;

void uart_init(unsigned int ubrr)
{

    /*Set baud rate */
    UBRR0H = (unsigned char)(ubrr >> 8); //write higher
    UBRR0L = (unsigned char)ubrr; //write lower bit

    /*Enable transmitter - enables interrupt */
    UCSR0B = _BV(TXEN0) | _BV(RXEN0);

    /* Set frame format: 8data, 1stop bit */
    UCSR0C = (3 <<UCSZ00);
}

void initLED()
{
    DDRB |= _BV(D1);
    DDRB |= _BV(D2);
    DDRB |= _BV(D3);
    DDRB |= _BV(D4);
}

void anim_toggle()
{

    SET_BIT(PINB,D1);
    SET_BIT(PINB,D2);
    SET_BIT(PINB,D3);
    SET_BIT(PINB,D4);
    _delay_ms(100);
    SET_BIT(PINB,D1);
    SET_BIT(PINB,D2);
    SET_BIT(PINB,D3);
    SET_BIT(PINB,D4);
    _delay_ms(100);
}

/* LIGHTS animations */
void anim_snake()
{
    SET_BIT(PINB,D1);
    _delay_ms(100);
    SET_BIT(PINB,D2);
    _delay_ms(100);
    SET_BIT(PINB,D3);
    _delay_ms(100);
    SET_BIT(PINB,D4);
    _delay_ms(300);
    SET_BIT(PINB,D1);
    SET_BIT(PINB,D2);
    SET_BIT(PINB,D3);
    SET_BIT(PINB,D4);
}

void led_logged_anim()
{

    anim_snake();
    anim_snake();
    anim_snake();
    anim_toggle();
    anim_toggle();
    anim_toggle();
    anim_toggle();
}

void led_failed_anim()
{
    for(uint8_t i = 0; i < 7 ; i++)
        anim_toggle();
}


/* UART */
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

void fill_username()
{
    uart_printstr("\tusername: ");

    buf = 0;
    while( buf < 15 )
    {
        c = uart_rx();
        if(c == 127 && buf > 0) //del
        {
            buf--;
            uart_printstr("\b \b");

            continue;
        }
        if(c == '\r')
        {
            username[buf] = '\0';
            uart_printstr("\r\n");
            return;
        }
        else if((c >='a' && c <='z') || (c >='A' && c <='Z') || (c >='0' && c <='9'))
        {
            username[buf] = c;
            uart_tx(c);
            buf++;
        }
    
    }
    if(buf > 14)
    {
        uart_printstr("\r\n\tERROR to long!!!!\r\n");
        anim_toggle();
        anim_toggle();
        _delay_ms(500);
    }
}

void fill_password()
{
    uart_printstr("\tpasseword: ");

    buf = 0;
    while( buf < 15 )
    {
        c = uart_rx();
        if(c == 127 && buf > 0) //del
        {
            buf--;
            uart_printstr("\b \b");

            continue;
        }
        if(c == '\r')
        {
            password[buf] = '\0';
            uart_printstr("\r\n");
            return;
        }
        else if((c >='a' && c <='z') || (c >='A' && c <='Z') || (c >='0' && c <='9'))
        {
            password[buf] = c;
            uart_tx('*');
            buf++;
        }
    }
    if(buf > 14)
    {
        uart_printstr("\r\n\tERROR to long!!!!\r\n");
        anim_toggle();
        anim_toggle();
        _delay_ms(500);
    }
}

int str_comp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int main(void)
{

    uint8_t logged = 0;
    uart_init(UBRRnL);
    initLED();

    while(!logged)
    {
            uart_printstr("Enter your login : \r\n");
            fill_username();
            fill_password();
            logged = ((!str_comp(username, "amaya")) && (!str_comp(password,"lula")));
            if(logged)
            {
                uart_printstr("Hello amaya!\r\nShall we play a game?\r\n");
                led_logged_anim();
            }
            else
            {
                uart_printstr("Bad combinaison username/password\r\n");
                led_failed_anim();
            }
    }
}

