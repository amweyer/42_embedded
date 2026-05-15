
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "mapping.h"
#include "helper.h"
#include "uart.h"

#include "i2c.h"
#include "PCA9555pw.h"


uint8_t num[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN,EIGHT, NINE};


void init_timer()
{
    TCCR1B |=  ( 1 << WGM12) | ( 1 << CS12 ) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    OCR1A = 15625; //1sec
}

void PCA955_init()
{

    PCA955_set_register(CONFIGURATION_PORT_0, ~BIT_7);
    PCA955_set_register(OUTPUT_PORT_0, ~BIT_7);
    
}

int main()
{
    cli();
    uart_init(UBRRnL);
    i2c_init();
    init_timer();
    PCA955_init();
    sei();

    while (1);

}


__attribute__((signal))
void __vector_11(void) //on compare match A
{
    static int i = 0;
    PCA955_set_register(CONFIGURATION_PORT_1, 0);
    
    PCA955_set_register(OUTPUT_PORT_1, num[i++, i%=10]);
}
