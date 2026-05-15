
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


void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE))
    ;
    /* Set up address and Data Registers */
    EEAR = uiAddress;
    EEDR = ucData;
    /* Write logical one to EEMPE */
    EECR |= (1<<EEMPE);
    /* Start eeprom write by setting EEPE */
    EECR |= (1<<EEPE);
}


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

uint8_t atoi_hex(char *clr)
{
	uint8_t res = 0;
	for (uint8_t i = 0; i < 2; i++)
	{
		if (clr[i] >= 'A' && clr[i] <= 'F')
			res = res * 16 + (clr[i] - 'A' + 10);
		else
			res = res * 16 + (clr[i] - '0'); 
	}
	return res;
}

uint8_t printable_char(char c)
{
    if(c >= 32 && c <= 126)
        return 1;
    return 0;
}

void process_changed_line(uint16_t addr, uint16_t changed_address)
{
    uint8_t line[16];
    for (uint8_t i = 0; i < 16 ; i++)
    {

        if (addr + i == changed_address)
        {
            uart_printstr("\x1b[38;5;120m");

        }
        if (addr + i < 1000)
            line[i] = EEPROM_read(addr+i);
        else
            line[i] = 0;
        print_hex(line[i]);
        uart_tx(' ');
        uart_printstr("\x1b[0m");
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

uint8_t check_values(uint8_t *val, uint16_t max)
{
    if (atoi_hex(val) < max)
        return (1);
    return (0);
}

uint8_t is_valid_command(uint8_t *command, uint8_t *addr, uint8_t *val)
{
 
    uint8_t i = 0;
    while(command[i] && command[i] != ' ')
    {
        addr[i] = command[i];
        i++;
    }
    if((!command[i]) || (command[i] && command[i] != ' '))
        return false;
    i++;
    uint8_t j = 0;
    while(command[i])
    {
        val[j] = command[i];
        i++;
        j++;
    }
    return (check_values(val, 256) && check_values(addr, 1000));

}


void fill_command(uint8_t *command)
{

    char c;
    uint8_t buf = 0;
    while(1)
    {
        c = uart_rx();
        if(c == 127 && buf > 0) //del
        {
            buf--;
            uart_printstr("\b \b");

            continue;
        }
        if(c == '\r')
        {
            command[buf] = '\0';
            uart_printstr("\r\n");
            return;
        }
        else if(((c >='a' && c <='f') || (c >='A' && c <='F') || (c >='0' && c <='9') || (c == ' ')) && buf < 15)
        {
            command[buf] = c;
            uart_tx(c);
            buf++;
        }
    
    }
  
}


void to_upper_16(char *str)
{
    for (int i = 0; i < 16; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
            str[i] -= 32;
    }
}

int main()
{
    uart_init(UBRRnL);
    char command[16];
    uint8_t addr[8];
    uint8_t val[3];
    // uint8_t val

    while(1)
    {
        uart_printstr("Enter an address of EEPROM and a char: ");

        fill_command(command);
        to_upper_16(command);
        if(is_valid_command(command, addr, val))
        {
            EEPROM_write(atoi_hex(addr), atoi_hex(val));
            for (uint16_t i = 0; i < 1000; i+= 16)
            {
                print_address(i);
                process_changed_line(i, atoi_hex(addr));
                uart_printstr("\r\n");
            }
        }
        else
            uart_printstr("Not valid !\r\n");

    }

}