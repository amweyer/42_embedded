
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "mapping.h"
#include "helper.h"

// 14.3.1 Alternate Functions of Port B

/* PB0 - PCINT0
    SW1 - INT0 */

uint8_t val = 0;

void initIO()
{
    //LED
    DDRB |= _BV(D1) | _BV(D2) | _BV(D3) | _BV(D4); //set leds
    PORTB &= ~(_BV(D1) | _BV(D2) | _BV(D3) | _BV(D4)) ;

    //SW1 & SW2
    /* SW1 - PD2 - INT0
        SW2 - PD4 -> PCINT20 */

    DDRD &= ~((1 << SW1) | (1 << SW2)); //SW1 boutton en output
    PORTD |= (1 << SW1) | (1 << SW2); //pull up - 5v


    //SW1
    EICRA |= _BV(ISC01); //The falling edge of INT0 generates an interrupt request
    EIMSK |= _BV(INT0);  //enable SW1 iterrupt

    //debounce timer 1
    TCCR1B |= _BV(WGM12); //set timer1 for debounce CTC mode - prescale 256
    OCR1A = 750; //13ms

    //SW2
    PCICR |= (1 << PCIE2); //enabling interrupt pins 16-23
    PCMSK2 |= (1 << PCINT20); //enabling specifically pin 20

    //debounce timer 0
    TCCR0A |= _BV(WGM01); //set tinmer 0 for deboiunce CTC -prescale 1024
    OCR0A = 203; //13ms

}

void show_led(uint8_t value)
{
    value |=  ((1 & (value >> 3)) << 4); //mets le bit de 3 en 4
    value &= ~(1 << 3); // suppime bit 3;

    PORTB = value;
}


void sw1_routine()
{
    TCNT1 = 0; //initlise le compteur
    UNSET_BIT(EIMSK, INT0); //disable interrupt on switch during debounce time 

    if (!(EICRA  & _BV(ISC00)))
        show_led(++val);
    TIMSK1 |= (1 << OCIE1A); //start timer 1 interrupt 
    TCCR1B |= (1 << CS12); // launch debounce timer;
    // SET_BIT(TCCR1B, CS12); 
}


void sw2_routine()
{

    TCNT0 = 0; //initlise le compteur

    PCMSK2 &= ~(1 << PCINT20); //disabnle sw1 interrupt  
    if(!(PIND & (1 << PD4))) //check if pressed
        show_led(--val);

    TIMSK0 |= (1 << OCIE0A); //start timer 0;
    TCCR0B |= (1 << CS02) | (1 << CS00); //launch timer precsaling on 1024

}

void sw1_end_routine()
{
    TIMSK1 &= ~(1 << OCIE1A); //stop timer interrupt
    UNSET_BIT(TCCR1B, CS12); //stop the debounce timer;

    EIFR |= _BV(INTF0);  // clean the interrupts due to bounce
    TOGGLE_BIT(EICRA, ISC00); //if was on falling edge becomes rising 
    SET_BIT(EIMSK, INT0); //enable SW1 iterrupt
}

void sw2_end_routine()
{
    TIMSK0 &= ~(1 << OCIE0A); //stop timer interrupt
    TCCR0B &= ~((1 << CS02) | (1 << CS00));// stop the debounce timer;

    PCIFR |= (1 << PCIF2); // clean the interruputs in bounce 
    SET_BIT(PCMSK2, PCINT20); //enable SW1 iterrupt
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
    sw1_routine();
}

__attribute__((signal))
void __vector_11(void) //on compare match A
{
    sw1_end_routine();
}


__attribute__((signal, used))
void __vector_5() //vector 6 PCINT2 pour pcint20 interrupt
{
    sw2_routine();
}

__attribute__((signal))
void __vector_14(void) //Timer/Counter0 Compare Match A
{
    sw2_end_routine();
}