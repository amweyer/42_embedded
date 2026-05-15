#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_R PD5
#define LED_G PD6
#define LED_B PD3

#define UART_BAUDRATE 115200
#define UBRRN_ 8


uint8_t red;
uint8_t green;
uint8_t blue;
char color_str[8];


//table 20.7
void uart_init()
{
    UBRR0 = 0;
    
    //init                              complete rec interr enabled
    UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0); // enable transmit
    UCSR0C &= ~(1<<USBS0); // 1 stop bit, useless as it's already 0 but..
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01); // 8bit
    
    UBRR0 = UBRRN_;
}

void init_rgb()
{
	DDRD |= (1<<LED_R) | (1<<LED_G) | (1<<LED_B);


	// RED && GREEN
	TCCR0A	|= (1<<COM0A1) | (1<<COM0B1) | (1<<WGM00) | (1<<WGM01);
	TCCR0B |= (1<<CS02) | (1<<CS00); // prescale 1024
	OCR0A = 255;//green
	OCR0B = 255;//red

	// BLUE mode 3 Fast PWM
	TCCR2A	|= (1<<COM2B1) | (1<<WGM21) | (1<<WGM20);
	TCCR2B |= (1<<CS20) | (1<<CS21) | (1<<CS22); // prescale 1024
	OCR2B = 255;//blue
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	//rouge PD5 OC0B
	//green PD6  OC0A
	//blue PD3	OC2B
	OCR0A = g;
	OCR0B = r;
	OCR2B = b;
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

void my_delay_ms(int time_ms)
{
    while (time_ms)
    {
        _delay_ms(10);
        time_ms -= 10;
    }
}

void blink(int color, int time1, int time2, int rep)
{
    DDRD |= (1<<DDD5) | (1<<DDD6);
    while (rep--)
    {
        PIND |= (1<<color);
        my_delay_ms(time1);
        PIND |= (1<<color);
        my_delay_ms(time2);
    }
}

int valid_char(unsigned char c)
{
    if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9') || c == '#')
        return (1);
    return (0);
}

void uart_getstr(char field[8])
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
        else if (i < 7 && valid_char(tmp))//VALIDCHAR TO ADD
        {
            //WRITE
            field[i++] = tmp;
            //ECHO
            uart_tx(tmp);
        }
        else //ERROR
            blink(LED_R, 300, 0, 1);
    }
    field[i] = 0;//null terminate
    uart_printstr("\r\n");//newline
}

int valid_color_str()
{
	if (color_str[0] != '#')
		return (0);
	for (uint8_t i = 1; i < 7; i++)
	{
		if (!color_str[i])
			return (0);
		if (color_str[i] == '#')
			return (0);
	}
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

void fill_colors()
{
	red = atoi_hex(&color_str[1]);
	green = atoi_hex(&color_str[3]);
	blue = atoi_hex(&color_str[5]);
}

void to_upper(char *str)
{
	while (*str++)
	{
		if (*str >= 'a' && *str <= 'z')
			*str -= 32;
	}
}

void get_input()
{
	int valid = 0;
	while (!valid)
	{
		uart_getstr(color_str);
		if (valid_color_str())
			valid++;
		else
			uart_printstr("ERROR\r\n");
	}
	to_upper(color_str);
	fill_colors();
}

int main()
{

	uart_init();
    init_rgb();
    while(1)
	{
		get_input();
        set_rgb(red, green, blue);
		_delay_ms(2);
	}
}


