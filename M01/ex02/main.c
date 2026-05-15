
#include <avr/io.h>
#include "mapping.h"
#include "helper.h"



/* 16.11 Register Description */

int main(void)
{


    DDRB |= _BV(D2);

    TCCR1A |= _BV(COM1A1) | _BV(WGM11) ;  //Toggle OC1A on Compare Match
    TCCR1B |= _BV(CS12) | _BV(WGM13) | _BV(WGM12) ; //prescaler to 256  / mode 14 fast PWM 

    ICR1 = 62500; //val to get 0.5s 
    OCR1A = 6250;

    while(1)
    {
        

    }
}
