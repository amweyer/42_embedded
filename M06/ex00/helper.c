
#include "helper.h"

void print_data(char *data)
{
    for(int i = 0; i < 7; i++)
    {
        print_hex_value(data[i]);
        uart_printstr(" ");
    }
    uart_printstr("\r\n");
}


void print_hex_value(char c)
{
    print_hex(c);
}

void  print_hex(uint8_t val)
{

    uart_tx((val/16 > 9) ? ('A' + val/16 - 10) : ('0' + val/16));
    uart_tx((val%16 > 9) ? ('A' + val%16 - 10) : ('0' + val%16));
        
}


void print_status(char str[25])
{
    if(DEBUG)
    {
        uart_debug(str);
        uart_debug("     Status: 0x");
        print_hex_value(TWSR & 0b11111000);
        uart_debug("\r\n");
    }
}