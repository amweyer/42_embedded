
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "mapping.h"
#include "helper.h"
#include "uart.h"
#include "clr.h"



/*

conv complete ADIF is hight
conv can be found at ADCL ADCH

RV1 - ADC POT => PCO/ADC0 => MUX00
LDR (R14) => ADC_LDR => ADC1
NTC (R20) => ADC_NTC => ADC2

need clock freq between 20kHz et 200kHz => 125 000 with prescaler 128

*/

void init_leds()
{
    //LED
    DDRB |= _BV(D1) | _BV(D2) | _BV(D3) | _BV(D4); //set leds
    PORTB &= ~(_BV(D1) | _BV(D2) | _BV(D3) | _BV(D4)) ;
}

void init_ADC_sensor()
{
    ADMUX   |= (1 << REFS0) //avcc ref and mux 000
            | (1 << ADLAR); // left adjust

    ADCSRA  |= (1 << ADEN) // enable ADC
            | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //PRECSALE 128
}

void initIO()
{
   init_ADC_sensor();
   init_leds();
   init_rgb();
}


uint8_t ADC_read_RV1()
{
    ADCSRA |= (1 << ADSC); //start conversion in single conevrsion mode;
    
    while((ADCSRA & (1 << ADSC))) ;
    return ADCH;
}

void clear_leds()
{
    PORTB &= ~(_BV(D1) | _BV(D2) | _BV(D3) | _BV(D4)) ;
}

void show_percent_LED(uint8_t val)
{

    clear_leds();
    val /= 63; //0.25*255    
    switch (val)
    {
    case 4:
        SET_BIT(PORTB, D4);
    case 3:
        SET_BIT(PORTB, D3);
    case 2:
        SET_BIT(PORTB, D2);
    case 1:
        SET_BIT(PORTB, D1);
    default:
        break;
    }
}

int main()
{
    initIO();

    while (1)
    {
        uint8_t val = ADC_read_RV1(); 
        wheel(val);
        show_percent_LED(val);
        _delay_ms(20);
    }
    
}