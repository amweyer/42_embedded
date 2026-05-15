
#include <avr/io.h>
#include <util/delay.h>

/*
DDRx direction pof the pin
If DDxn is written logic one, Pxn is configured as an output
pin. If DDxn is written logic zero, Pxn is configured as an input pin.

*/

#define LED_ON  PORTB |= (1<<PB0)
#define LED_OFF PORTB &= ~(1<<PB0)

int main(void)
{
    DDRB |= (1 << PB0); ///* Define directions for port pins on on */
    PORTB |= (1 << PB0); //Define pull-ups and set outputs high

    DDRD &= ~(1 << PD2); //PD2 boutton en output
    PORTD |= (1 << PD2); //pull up - 5v

    while(1)
    {
        if(!(PIND & (1<<PIND2))) //check that the val 
        {
            _delay_ms(100);
            LED_ON;
        }
        else
        {
            _delay_ms(100);
            LED_OFF; 
        }
    }
}
