
#include <avr/io.h>
#include <util/delay.h>

#include "mapping.h"
#include "helper.h"


#define RED 1
#define GREEN 2
#define BLUE 3
#define YELLOW 4
#define CYAN 5
#define MAGENTA 6 
#define WHITE 7


inline void init_rgb()
{
    DDRD |= _BV(LED_B) | _BV(LED_G) | _BV(LED_R);
}

inline void clearRGB()
{
    PORTD &= ~(_BV(LED_B) | _BV(LED_G) | _BV(LED_R));
}

void set_rgb(uint8_t R, uint8_t G, uint8_t B)
{
    if(R==1)
        SET_BIT(PORTD, LED_R);
    if(G==1)
        SET_BIT(PORTD, LED_G);
    if(B==1)
        SET_BIT(PORTD, LED_B);
}

void light(uint8_t color)
{
    clearRGB();

    if(color == RED)
        set_RGB(1,0,0);
    if(color == GREEN)
        set_RGB(0,1,0);
    if(color == BLUE)
        set_RGB(0,0,1);
    if(color == YELLOW)
        set_RGB(1,1,0);
    if(color == CYAN)
        set_RGB(0,1,1);
    if(color == MAGENTA)
        set_RGB(1,0,1);
    if(color == WHITE)
        set_RGB(1,1,1);
    _delay_ms(1000);
}

int main(void)
{
    init_rgb();
    while(1)
    {
        for(uint8_t color = 1; color < 8; color++ )
            light(color);
    }
}

