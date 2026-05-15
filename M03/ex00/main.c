
#include <avr/io.h>
#include <util/delay.h>

#include "mapping.h"
#include "helper.h"



inline void initRGB()
{
    DDRD |= _BV(LED_B) | _BV(LED_G) | _BV(LED_R);
}


int main(void)
{
    initRGB();
    while(1)
    {
        SET_BIT(PIND, LED_R);
        _delay_ms(1000);
        SET_BIT(PIND, LED_R);
        SET_BIT(PIND, LED_G);
        _delay_ms(1000);
        SET_BIT(PIND, LED_G);
        SET_BIT(PIND, LED_B);
        _delay_ms(1000);
        SET_BIT(PIND, LED_B);
    }
}

