
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "mapping.h"
#include "helper.h"

#define STEP 63

void initIO()
{
    //LED D2
    DDRB |= _BV(D2); //set led D2
    PORTB &= ~_BV(D2);

    //TIMER0 : interruption periodique - mode CTC -- 64*250*1000=16*10**6
    TCCR0A |= _BV(WGM01); //CTC mode
    TCCR0B |= _BV(CS02) ; //prescaler 256
    OCR0A = 122; // 512 pas de 122 tics = 1sec 

    //Timer1 : controle rapport cyclique PB1 - PWM
    TCCR1A |= _BV(COM1A1) | _BV(WGM11); //clear OC1A on compare match | fast PWM
    TCCR1B |= _BV(WGM12) | _BV(WGM13) |  _BV(CS11); //fast PWM config |  prescale 8 
    TIMSK0 |= _BV(OCIE1A); //comnapr match A

    ICR1 = 16000; //1s - 125Hz 
    /* setp 16 000/256 */
    OCR1A = 0;  //start at 0
}

int main(void)
{
    initIO();
    SET_BIT (SREG,SREG_I); //    sei();

    while(1) {}
}

__attribute__((signal))
void __vector_14(void) //on compare match A
{
    static uint16_t c;

    c++;
    if(c == 512)
        c = 0;
    if(c < 256)
        OCR1A += STEP;
    else
        OCR1A -= STEP;
}

