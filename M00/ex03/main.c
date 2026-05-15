
#include <avr/io.h>
#include <util/delay.h>

/*
DDRx direction pof the pin
If DDxn is written logic one, Pxn is configured as an output
pin. If DDxn is written logic zero, Pxn is configured as an input pin.

*/

#define LED_ON  PORTB |= (1<<PB0)
#define LED_OFF PORTB &= ~(1<<PB0)
#define DEBOUNCE_TIME 25 //definie un temps de debounce en us
#define LOCK_INPUT_TIME 250 /* time to wait after a button press */

int main(void)
{
    DDRB |= (1 << PB0); // defines output on led
    // PORTB |= (1 << PB0); //starts with light
    
    DDRD &= ~(1 << PD2); //PD2 boutton en output
    PORTD |= (1 << PD2); //pull up - 5v


    while(1)
    {
        if(bit_is_clear(PIND,PD2))
        {

            _delay_ms(DEBOUNCE_TIME);
            if(bit_is_clear(PIND,PD2))
            {
                PORTB ^= (1<<PB0);
                loop_until_bit_is_set(PIND,PD2);
            }
        }
    }
            
}
