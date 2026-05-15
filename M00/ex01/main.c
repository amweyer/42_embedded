
#include <avr/io.h>

/*
DDRx direction pof the pin
If DDxn is written logic one, Pxn is configured as an output
pin. If DDxn is written logic zero, Pxn is configured as an input pin.

// PINB |= (1 << PINB5); //toggle pin

*/

#define LED_PIN PB0

int main(void)
{
    DDRB |= (1 << LED_PIN); ///* Define directions for port pins on on */
    PORTB |= (1 << LED_PIN); //Define pull-ups and set outputs high
    
    while(1){}
}
