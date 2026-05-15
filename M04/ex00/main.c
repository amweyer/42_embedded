
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "mapping.h"
#include "helper.h"

// 14.3.1 Alternate Functions of Port B

/* PB0 - PCINT0
    SW1 - INT0 */

void initIO()
{
    DDRB |= _BV(D1); //set d1
    PORTB &= ~_BV(D1);

    // PORTB |= _BV(D1);


    DDRD &= ~(1 << SW1); //SW1 boutton en output
    PORTD |= (1 << SW1); //pull up - 5v

    EICRA |= _BV(ISC01); //The falling edge of INT0 generates an interrupt request
    EIMSK |= _BV(INT0);  //enable SW1 iterrupt

    TCCR1B |= _BV(WGM12); //set timer1 for debounce CTC mode - prescale 256
    OCR1A = 750; //13ms

    
}

void start_debounce_timer()
{
    UNSET_BIT(EIMSK, INT0); //disable interrupt on switch during debounce time 
    TCNT1 = 0; //initlise le compteur

    if (!(EICRA  & _BV(ISC00)))
        TOGGLE_BIT(PORTB, D1);  //toggle the light

    TIMSK1 |= (1 << OCIE1A); //start timer 1 interrupt 
    SET_BIT(TCCR1B, CS12); // launch debounce timer;
}

void end_debounce_timer()
{
    TIMSK1 &= ~(1 << OCIE1A); //stop timer interrupt
    UNSET_BIT(TCCR1B, CS12); //stop the debounce timer;

    EIFR |= _BV(INTF0);  // clean the interrupts due to bounce
    TOGGLE_BIT(EICRA, ISC00); //if was on falling edge becomes rising 
    SET_BIT(EIMSK, INT0); //enable SW1 iterrupt
}



int main(void)
{

    initIO();
    SET_BIT (SREG,SREG_I); //    sei();

    while(1)
    {

    }
}

__attribute__((signal, used))
void __vector_1() //vector 2 INT0 interrupt
{
    start_debounce_timer();
}

__attribute__((signal))
void __vector_11(void) //on compare match A
{
    end_debounce_timer();
}

