
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
LDR (R14) => ADC_LDR => ADC1
NTC (R20) => ADC_NTC => ADC2

need clock freq between 20kHz et 200kHz => 125 000 with prescaler 128

*/


void initIO()
{
    ADMUX   |= (1 << REFS0) | (1 << REFS1) //internal 1.1V
            | (1 << MUX3); //ADC8 - Temperature sensor

    ADCSRA  |= (1 << ADEN) // enable ADC
            | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //PRECSALE 128
}

void  print(int val)
{
    int flag = 0;
    for(uint16_t i = 1000; i > 0 ; i/=10 )
    {
        if(val / i != 0)
            flag = 1;          
        if((val/i==0) && (!flag) && (i != 1))
            continue;
        uart_tx(val/i  + '0');
        val %= i;
    }
}

void show_temp()
{
    ADCSRA |= (1 << ADSC); //start conversion in single conevrsion mode;
    
    while((ADCSRA & (1 << ADSC))) ;
    int tmp = (ADC - 324)*100/122;
    print(tmp);
    uart_printstr("\r\n");

}


int main()
{
    initIO();
    uart_init(UBRRnL);
    
    ADCSRA |= (1 << ADSC); //start conversion in single conevrsion mode;
    while(!(ADCSRA & (1 << ADSC))) ;

    while (1)
    {
        show_temp();
        _delay_ms(20);
    }
    
}