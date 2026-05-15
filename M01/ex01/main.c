
#include <avr/io.h>
#include "mapping.h"
#include "helper.h"



/* 16.11 Register Description */

int main(void)
{


    DDRB |= _BV(D2);

    TCCR1A |= _BV(COM1A0);  //Toggle OC1A on Compare Match
    TCCR1B |= _BV(CS12) | _BV(WGM12) ; //prescaler to 256 to slow down the clock / mode CTC => qui overfow
    
    OCR1A = 31250; //val to get 0.5s 

    while(1)
    {

    }
}
