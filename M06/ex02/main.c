
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "mapping.h"
#include "helper.h"
#include "uart.h"

#include "aht20.h"
#include "i2c.h"


#define READ 0
#define WRITE 1



void print_status(char str[25]);
void i2c_write(unsigned char data);
void print_hex_value(char c);
void  print_hex(uint8_t val);


int main()
{
    uart_debug("INIT: \r\n");
    uart_init(UBRRnL);
    i2c_init();

    aht20_calibrate();
    while (1)
    {
        aht20_trigger_measurement();
        ath20_read_data();

        _delay_ms(80);
    }
}

