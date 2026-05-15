
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "mapping.h"
#include "helper.h"
#include "uart.h"

/*

conv complete ADIF is hight
conv can be found at ADCL ADCH

RV1 - ADC POT => PCO/ADC0 => MUX00

need clock freq between 20kHz et 200kHz => 125 000 with prescaler 128

*/

void initIO()
{
    ADMUX   |= (1 << REFS0) //avcc ref and mux 000
            | (1 << ADLAR); // left adjust

    ADCSRA  |= (1 << ADEN) // enable ADC
            | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //PRECSALE 128
}

void  print_hex(uint8_t val)
{
    uart_tx((val/16 > 9) ? ('a' + val/16 - 10) : ('0' + val/16));
    uart_tx((val%16 > 9) ? ('a' + val%16 - 10) : ('0' + val%16));
    uart_printstr("\r\n");
}

void show_RV1()
{
    ADCSRA |= (1 << ADSC); //start conversion in single conevrsion mode;
    
    while((ADCSRA & (1 << ADSC))) ;
    print_hex(ADCH);
}

int main()
{
    initIO();
    uart_init(UBRRnL);

    
    // ADCSRA |= (1 << ADSC); //start conversion in single conevrsion mode;
    // while(!(ADCSRA & (1 << ADSC))) ;

    while (1)
    {
        show_RV1();
        _delay_ms(20);
    }
    
}