#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "mapping.h"
#include "helper.h"

void init_rgb()
{
	DDRD |= (1<<LED_R) | (1<<LED_G) | (1<<LED_B);


	// RED && GREEN
	TCCR0A	|= (1<<COM0A1) | (1<<COM0B1)
			| (1<<WGM00)
			| (1<<WGM01);
	
	TCCR0B |= (1<<CS02) | (1<<CS00); // prescale 1024
	
	OCR0A = 255;//green
	OCR0B = 255;//red

	// BLUE mode 3 Fast PWM
	TCCR2A	|= (1<<COM2B1)
			| (1<<WGM21)
			| (1<<WGM20);
	
	TCCR2B |= (1<<CS20) | (1<<CS21) | (1<<CS22); // prescale 1024
	OCR2B = 255;//blue
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	//rouge PD5 OC0B
	//green PD6  OC0A
	//blue PD3	OC2B
	OCR0A = g;
	OCR0B = r;
	OCR2B = b;
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85){
    	set_rgb(255 - pos * 3, 0, pos * 3);
    }
	else if (pos < 170) {
    	pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
	}
    else{
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

// int main()
// {
// 	uint8_t color = 0;

//     init_rgb();
//     while(1)
// 	{
//         wheel(color++);
// 		_delay_ms(2);
// 	}
// }


