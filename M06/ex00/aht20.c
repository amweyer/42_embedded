
#include "aht20.h"

void aht20_calibrate()
{
    // uart_debug("sensor calibration\r\n");

    uint8_t status;

    i2c_start();
    i2c_address(SLA_W);
    i2c_write(0x71);
    i2c_stop();
    _delay_ms(10);

    i2c_start();
    i2c_address(SLA_R);
    i2c_read(1);
    status = TWDR;
    i2c_stop();
    
    if(!(status & 0x08))
    {
        print_status("aht20_calibrate");
        i2c_start();
        i2c_address(SLA_W);
        i2c_write(0xBE);
        i2c_write(0x08);
        i2c_write(0x00);
        i2c_stop();
    }
    _delay_ms(10);
}


void aht20_trigger_measurement()
{

    uart_debug("trigger measurements\r\n");

    i2c_start();
    i2c_write(SLA_W);
    
    _delay_ms(10);
    i2c_write(0xAC);
    i2c_write(0x33);
    i2c_write(0x00);
    // _delay_ms(80);

    i2c_stop();
    
    _delay_ms(80);

    // print_status();
}



void ath20_read_data(void)
{
    uint8_t data[7];

    print_status("ath20_read_data");
    i2c_start();

    i2c_write(SLA_R);
    
    for(int i = 0; i < 7; i++)
    {
        i2c_read(i==6);
        data[i] = TWDR;
    }

    i2c_stop();

    print_data(data);

}


