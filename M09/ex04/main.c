
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "mapping.h"
#include "helper.h"
#include "uart.h"

#include "i2c.h"
#include "PCA9555pw.h"


uint8_t led_id[] = {DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3};
uint8_t num[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN,EIGHT, NINE};
char    screen[4] = {0, 0, 4, 2};


void init_timer_0()
{
    TCCR0A |= (1 << WGM01); 
    TCCR0B |= (1 << CS01);
    TIMSK0 |= (1 << OCIE0A);
    OCR0A = 1;
}

void PCA955_init()
{

    PCA955_set_register(CONFIGURATION_PORT_0, ~(DIGIT_0 | DIGIT_1 | DIGIT_2 | DIGIT_3));
    // PCA955_set_register(CONFIGURATION_PORT_0, ~( DIGIT_3));
    
    PCA955_set_register(CONFIGURATION_PORT_1, 0);
}

int main()
{
    cli();
    uart_init(UBRRnL);
    i2c_init();
    PCA955_init();
    init_timer_0();
    sei();
    while (1);
}

__attribute__((signal))
void __vector_14(void) //on compare match A timer 0
{
    static int i = 2;
    
    TIMSK0 &= ~(1 << OCIE0A);
    PCA955_set_register(OUTPUT_PORT_1, 0);
    PCA955_set_register(OUTPUT_PORT_0, ~led_id[i]);
    PCA955_set_register(OUTPUT_PORT_1, num[screen[i]]);
    if (++i == 4)
        i = 2;
    TIMSK0 |= (1 << OCIE0A);
}
