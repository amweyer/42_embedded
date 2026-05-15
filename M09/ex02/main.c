
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#include "mapping.h"
#include "helper.h"
#include "uart.h"

#include "i2c.h"
#include "PCA9555pw.h"





void print_status(char str[25]);
void i2c_write(unsigned char data);
void print_hex_value(char c);
void  print_hex(uint8_t val);


int main()
{
    uart_debug("INIT: \r\n");
    uart_init(UBRRnL);
    i2c_init();

    // while (1)
    {
        PCA955_set_register(CONFIGURATION_PORT_0, ~BIT_7);
        PCA955_set_register(OUTPUT_PORT_0, ~BIT_7);

        PCA955_set_register(CONFIGURATION_PORT_1, 0);
        PCA955_set_register(OUTPUT_PORT_1, FOUR);

        _delay_ms(80);
    }
}

