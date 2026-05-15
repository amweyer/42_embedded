
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "mapping.h"
#include "helper.h"
#include "uart.h"


/* max freq 30MHZ 16MHZ*/

void SPI_MasterInit(void)
{
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1<<MOSI)|(1 << SCK) | (1<<SS);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

uint8_t SPI_masterTxRx(uint8_t data)
{
    // transmit data
    SPDR = data;

    // Wait for reception complete
    while(!(SPSR & (1 << SPIF)));

    // return Data Register
    return SPDR;
}

void init_timer1()
{

    //Timer1 : CTC
    TCCR1A |= _BV(COM1A1);
    TCCR1B |= _BV(WGM12) | _BV(CS11); //fast PWM config |  prescale 8 
    // TIMSK0 |= _BV(OCIE1A); //comnapr match A

    OCR1A = 5000;  //start at 0
}


void SPI_StartTransmission()
{
    SPI_masterTxRx(0);
    SPI_masterTxRx(0);
    SPI_masterTxRx(0);
    SPI_masterTxRx(0);
}

void SPI_EndTransmission()
{
    SPI_masterTxRx(255);
    SPI_masterTxRx(255);
    SPI_masterTxRx(255);
    SPI_masterTxRx(255);
}

void set_led(char r, char g, char b, char global)
{
    SPI_masterTxRx((0b11100000 | (global>>3))); //divbison par 8
    SPI_masterTxRx(b);
    SPI_masterTxRx(g);
    SPI_masterTxRx(r);
}


int main()
{
    char red;
    char green;
    char blue;
    char led;

    SPI_MasterInit();
    uart_init(UBRRnL);
    init_timer1();

    SET_BIT(SREG,SREG_I); //    sei();
    while (1)
    {
        get_input();
        TIMSK1 &= ~_BV(OCIE1A); //comnapr match A
        if (!str_comp(color_str, "#FULLRAINBOW"))
        {
            TIMSK1 |= _BV(OCIE1A); //comnapr match A
        }
        else
        {
            red = atoi_hex(&color_str[1]);
            green = atoi_hex(&color_str[3]);
            blue = atoi_hex(&color_str[5]);
            for (int i = 6; i < 9; i++)
            {
                if (color_str[8] == i + '0')
                led = i;
            }
            
            SPI_StartTransmission();
            set_led(red, green ,blue ,22 * (led == 6));
            set_led(red, green ,blue ,22 * (led == 7));
            set_led(red, green ,blue ,22 * (led == 8));
            SPI_EndTransmission();
        }
    }
}

void set_rgb(char r, char g, char b)
{
    SPI_StartTransmission();
    set_led(r, g ,b ,22);
    set_led(r, g ,b ,22);
    set_led(r, g ,b ,22);
    SPI_EndTransmission();
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85){
    	set_rgb(255 - pos * 3, 0, pos * 3);
    }
	else if (pos < 170) {
    	pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
	}
    else{
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

__attribute__((signal))
void __vector_11(void) //on compare match A
{
    static uint8_t i = 0;
    wheel(i++);
}
