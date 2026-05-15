
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "mapping.h"
#include "helper.h"


/* max freq 30MHZ 16MHZ*/

void SPI_MasterInit(void)
{
    /* Set MOSI and SCK output, all others input */
    DDR_SPI = (1<<MOSI)|(1 << SCK) | (1<<SS);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

uint8_t SPI_masterTxRx(uint8_t data)
{
    // transmit data
    SPDR = data;

    // Wait for reception complete
    while(!(SPSR & (1 << SPIF)));

    // return Data Register
    return SPDR;
}


void SPI_StartTransmission()
{
    SPI_masterTxRx(0);
    SPI_masterTxRx(0);
    SPI_masterTxRx(0);
    SPI_masterTxRx(0);
}

void SPI_EndTransmission()
{
    SPI_masterTxRx(255);
    SPI_masterTxRx(255);
    SPI_masterTxRx(255);
    SPI_masterTxRx(255);
}

void set_led(char r, char g, char b, char global)
{
    SPI_masterTxRx((0b11100000 | (global>>3))); //divbison par 8
    SPI_masterTxRx(b);
    SPI_masterTxRx(g);
    SPI_masterTxRx(r);
}

void init_ADC_sensor()
{
    ADMUX   |= (1 << REFS0) //avcc ref and mux 000
            | (1 << ADLAR); // left adjust

    ADCSRA  |= (1 << ADEN) // enable ADC
            | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //PRECSALE 128
}



uint8_t ADC_read_RV1()
{
    ADCSRA |= (1 << ADSC); //start conversion in single conevrsion mode;
    
    while((ADCSRA & (1 << ADSC))) ;
    return ADCH;
}

int main()
{
    SPI_MasterInit();
    init_ADC_sensor();

    while (1)
    {
        uint8_t val = ADC_read_RV1(); 
        SPI_StartTransmission();
        set_led(0, 0 ,167,22 * (val >= 255/3));
        set_led(0, 0 ,167,22 * (val >= (255/3)*2));
        set_led(0, 0 ,167,22 * (val == 255));
        SPI_EndTransmission();
    }
}