
#include <avr/io.h>
#include <util/delay.h>

/*
DDRx direction pof the pin
If DDxn is written logic one, Pxn is configured as an output
pin. If DDxn is written logic zero, Pxn is configured as an input pin.

*/

#define SET_LED(bit) PORTB |= (1 << bit)
#define UNSET_LED(bit) PORTB &= ~(1<<bit)

#define DEBOUNCE_TIME 25 //definie un temps de debounce en us


void from_int_to_bin(uint8_t val)
{

    for(uint8_t i = 0; i < 4; i++)
    {
        if(bit_is_set(val,i))
        {
            SET_LED((i + (i==3))); //si i==3 slors i=4
        }
        else{
            UNSET_LED((i + (i==3)));
        }
    }
}

int is_button_pressed(uint8_t pin)
{
    if(bit_is_clear(PIND,pin))
    {
        
        _delay_ms(DEBOUNCE_TIME);
        if(bit_is_clear(PIND,pin))
        {
            loop_until_bit_is_set(PIND,pin);
            return 1;
        }
        return 0;
    }
    return (0);
}

int main(void)
{
    uint8_t val = 0;

    DDRB |= (1 << PB0) | (1 << PB1) |(1 << PB2) |(1 << PB4)  ; // defines output on led
    // PORTB |= (1 << PB0); //starts with light
    
    DDRD &= ~((1 << PD2) | (1 << PD4));   // PD2 et PD4 en entrée
    PORTD |= (1 << PD2) | (1 << PD4);     // pull-up activé sur les deux

    while(1)
    {
        if (is_button_pressed(PD2))
            val++;
        if(is_button_pressed(PD4))
            val--;
        from_int_to_bin(val);
        _delay_ms(DEBOUNCE_TIME);

    }
}
