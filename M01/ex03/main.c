
#include <avr/io.h>
#include <util/delay.h>

#include "mapping.h"
#include "helper.h"

#define STEP 6250
#define VAL_1HZ 62500


/* 16.11 Register Description */

void initTimers()
{

    TCCR1A |= _BV(COM1A1) | _BV(WGM11) ;  //clear OC1A on Compare Match / Mode 14
    TCCR1B |= _BV(CS12) | _BV(WGM13) | _BV(WGM12) ; //prescaler to 256  / mode 14 fast PWM 

    ICR1 = VAL_1HZ; //val to get 1hz 
    OCR1A = STEP; //10%
}


void initIO()
{
    DDRB |= _BV(D2);

    DDRD &= ~((1 << PD2) | (1 << PD4));   // PD2 et PD4 en entrée
    PORTD |= (1 << PD2) | (1 << PD4);     // pull-up activé sur les deux
}

int is_button_pressed(uint8_t pin)
{
    if(bit_is_clear(PIND,pin))
    {
        
        _delay_ms(25);
        if(bit_is_clear(PIND,pin))
        {
            loop_until_bit_is_set(PIND,pin);
            return 1;
        }
        return 0;
    }
    return (0);
}

int main(void)
{
    initIO();
    initTimers();

    uint16_t max = VAL_1HZ;
    uint16_t duty_cycle = STEP;

    while(1)
    {
        if (is_button_pressed(PD2))
        {
            if(duty_cycle <= max - STEP)
                duty_cycle += STEP;
        }
        if(is_button_pressed(PD4))
        {
            if(duty_cycle - STEP >= STEP)
                duty_cycle -= STEP;
        }
        OCR1A = duty_cycle;
    }
}

