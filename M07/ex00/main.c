
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "mapping.h"
#include "helper.h"
#include "uart.h"


void process_line(uint16_t addr);
void  print_address(int val);
uint8_t printable_char(char c);
// void process_line(uint16_t addr)


unsigned char EEPROM_read(unsigned int uiAddress)
{
    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE))
    ;
    /* Set up address register */
    EEAR = uiAddress;
    /* Start eeprom read by writing EERE */
    EECR |= (1<<EERE);
    /* Return data from Data Register */
    return EEDR;
}

void  print_address(int val)
{
    // int flag = 0;
    for(uint64_t i = 4294967296; i > 0 ; i/=16 )
    {
        // if(val / i != 0)
        //     flag = 1;          
        // if((val/i==0) && (!flag) && (i != 1))
        //     continue;
        uart_tx("0123456789ABCDEF"[val/i]);
        val %= i;
    }
    uart_tx(' ');
}

uint8_t printable_char(char c)
{
    if(c >= 32 && c <= 126)
        return 1;
    return 0;
}

void process_line(uint16_t addr)
{
    uint8_t line[16];
    for (uint8_t i = 0; i < 16 ; i++)
    {

        if (addr + i < 1000)
            line[i] = EEPROM_read(addr+i);
        else
            line[i] = 0;
        print_hex(line[i]);
        uart_tx(' ');

    }

    uart_tx('|');
    for (uint8_t i = 0; i < 16 ; i++)
    {
        if(printable_char(line[i]))
            uart_tx(line[i]);
        else
            uart_tx('.');
    }
    uart_tx('|');

}

int main()
{
    uart_init(UBRRnL);
  
    for (uint16_t i = 0; i < 1000; i+= 16)
    {
        print_address(i);
        process_line(i);

        uart_printstr("\r\n");
    }

}