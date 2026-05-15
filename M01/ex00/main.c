
#include <avr/io.h>
#include "mapping.h"
#include "helper.h"


/* avr-objdump -d main.bin --disassemble=wait_500 > ass.md */


/* clk 16Mhz donc veut T = t/16M = 0.5

on veut tel que t=8 000 000
*/

// __attribute__((always_inline))
void wait_500(void)
{
    volatile uint16_t i, j; //empeche opti
    for(i = 0; i < 241; i++) //241*33195 = 7999995 = 8 000 000
    {
        for (j = 0; j < 2213; j++) //15 cycles => 15*2213 = 33195
        {

        }
    }
    return;
}


int main(void)
{

    SET_BIT(DDRB, D2);
    

    while(1)
    {
        wait_500();
        SET_BIT(PINB, D2);
    }
}
