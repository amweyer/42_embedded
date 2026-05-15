
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


// #define DEBUG 1


/*

- I2C mcu => SLC=100kHz p.222
 TWBR.preacle = 72 = 4*18
*/




int main()
{
    uart_init(UBRRnL);
    uart_debug("INIT: \r\n");
    i2c_init();
    i2c_start();
    // print_status("start");
    i2c_stop();
}

